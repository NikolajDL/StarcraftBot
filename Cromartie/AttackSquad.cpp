#include "AttackSquad.h"

#include "UnitTracker.h"
#include "BaseTracker.h"
#include "BorderTracker.h"
#include "BuildOrderManager.h"
#include "PlayerTracker.h"
#include "UnitHelper.h"
#include "GameProgressDetection.h"
#include "Logger.h"

AttackSquadTask::AttackSquadTask(ArmyBehaviour behaviour)
	: BaseSquadTask(behaviour)
	, mIsAttacking(false)
{
}

int AttackSquadTask::getEndTime() const
{
	return Requirement::maxTime;
}

int AttackSquadTask::getEndTime(Unit unit) const
{
	return Requirement::maxTime;
}

int AttackSquadTask::getPriority(Unit unit) const
{
	return unit->getType().isWorker() ? 5 : 20;
}

Position AttackSquadTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position AttackSquadTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool AttackSquadTask::preUpdate()
{
	updateRequirements();

	return hasEnded();
}

BorderPosition AttackSquadTask::getLargestChoke(const std::set<BorderPosition> &chokes)
{
	int largestChokeSize = 0;
	BorderPosition largestChoke;
	for each(BorderPosition border in chokes)
	{
		int chokeSize = border.mChoke->getClearance();
		if(chokeSize > largestChokeSize)
		{
			largestChoke = border;
			largestChokeSize = chokeSize;
		}
	}

	return largestChoke;
}

bool AttackSquadTask::update()
{
	Goal squadsGoal;


	// Just attack if its not mining anywhere
	bool hasMiningBases = false;
	int techStructures = 0;
	Base baseToDefend;
	bool baseUnderAttack = false;
	const int mySupply = BWAPI::Broodwar->self()->supplyUsed();
	for each(Base base in BaseTracker::Instance().getPlayerBases())
	{
		const int techBuildings = base->getNumberOfTechBuildings();
		if(!base->getEnemyThreats().empty() && (techBuildings > 0 || (!base->isMinedOut() && base->getResourceDepot())))
		{
			bool thisUnderAttack = base->isUnderAttack();
			if((thisUnderAttack && !baseUnderAttack) || (thisUnderAttack == baseUnderAttack && techBuildings > techStructures))
			{
				baseUnderAttack = thisUnderAttack;
				baseToDefend = base;
				techStructures = techBuildings;
			}
		}

		if(!base->getMinerals().empty() && base->getResourceDepot() && base->getResourceDepot()->exists())
			hasMiningBases = true;
	}

	UnitGroup avoidGroup;
	UnitGroup engageGroup;
	for each(const UnitGroup &unitGroup in PlayerTracker::Instance().getEnemyClusters())
	{
		engageGroup += unitGroup;
	}
	
	if(baseUnderAttack)
	{
		const int rating = mUnits.ratingDifference(baseToDefend->getEnemyThreats());
		if(rating > -1000)
			squadsGoal = Goal(ActionType::Defend, baseToDefend->getEnemyThreats(), avoidGroup);
	}

	Base bestBaseToAttack;
	if(mLastGoal.getGoalType() == GoalType::Base && mLastGoal.getActionType() == ActionType::Attack && mLastGoal.getBase()->isEnemyBase())
		bestBaseToAttack = mLastGoal.getBase();

	for each(Base base in BaseTracker::Instance().getEnemyBases())
	{
		if(!bestBaseToAttack || base->getBuildings().size() < bestBaseToAttack->getBuildings().size())
			bestBaseToAttack = base;
	}

	if(mIsAttacking)
	{
		if(bestBaseToAttack)
			squadsGoal = Goal(ActionType::Attack, bestBaseToAttack, engageGroup, avoidGroup);
		else if(squadsGoal.getGoalType() == GoalType::None)
			squadsGoal = Goal(ActionType::Attack, Position(BWAPI::Broodwar->mapWidth()*24, BWAPI::Broodwar->mapHeight()*16), engageGroup, avoidGroup);
		else if(!engageGroup.empty() && !avoidGroup.empty() && squadsGoal.getActionType() == ActionType::Attack && squadsGoal.getGoalType() == GoalType::Base)
			squadsGoal = Goal(ActionType::Attack, avoidGroup.getCenter(), engageGroup, avoidGroup);
		else 
			squadsGoal = Goal(ActionType::Attack, engageGroup, avoidGroup);
	}

	if(squadsGoal.getGoalType() == GoalType::None && baseToDefend && mUnits.canMajorityAttack(baseToDefend->getEnemyThreats()))
		squadsGoal = Goal(ActionType::Defend, baseToDefend->getCenterLocation(), engageGroup, avoidGroup);

	if(squadsGoal.getGoalType() == GoalType::None || mUnits.size()<=0)
	{
		if(!BorderTracker::Instance().getBorderPositions(PositionType::TechDefenseChokepoint).empty())
			squadsGoal = Goal(ActionType::Hold, getLargestChoke(BorderTracker::Instance().getBorderPositions(PositionType::TechDefenseChokepoint)).mChoke->getCenter(), engageGroup, avoidGroup);
	}

	if(squadsGoal.getGoalType() == GoalType::None || mUnits.size()<=0)
	{
		if(!BorderTracker::Instance().getBorderPositions(PositionType::DefenseChokepoint).empty())
			squadsGoal = Goal(ActionType::Hold, getLargestChoke(BorderTracker::Instance().getBorderPositions(PositionType::DefenseChokepoint)).mChoke->getCenter(), engageGroup, avoidGroup);
	}

	if(mLastGoal.getActionType() == ActionType::Attack && mLastGoal.getGoalType() == GoalType::Base && mLastGoal.getBase()->isEnemyBase())
	{
		if(squadsGoal.getActionType() != ActionType::Attack || (squadsGoal.getGoalType() == GoalType::Base && !squadsGoal.getBase()->isEnemyBase()))
		{
			mIsAttacking = false;
			mUnits += mSavedUnits;
			LOGMESSAGEWARNING(String_Builder() << "Failed base attacks increased");
		}
	}

	mLastGoal = squadsGoal;
	if(squadsGoal.getGoalType() != GoalType::None)
	{
		for(std::map<Unit, Behaviour>::iterator it = mUnitBehaviours.begin(); it != mUnitBehaviours.end(); ++it)
			it->second.update(squadsGoal, mUnits);
	}

	return hasEnded() && mUnits.empty();
}

bool AttackSquadTask::waitingForUnit(Unit unit) const
{
	return false;
}

void AttackSquadTask::giveUnit(Unit unit)
{
	if(unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
		mObserver = unit;
	
	if(mIsAttacking)
		mSavedUnits.insert(unit);
	else
		mUnits.insert(unit);


	mUnitBehaviours[unit] = Behaviour(unit);
}

void AttackSquadTask::returnUnit(Unit unit)
{

	mUnits.erase(unit);

	mUnitBehaviours[unit].onDeleted();
	mUnitBehaviours.erase(unit);

	if(unit == mObserver)
		mObserver = StaticUnits::nullunit;
}

bool AttackSquadTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return false;
}

UnitGroup AttackSquadTask::getFinishedUnits()
{
	return mUnits;
}

void AttackSquadTask::updateRequirements()
{
	clearRequirements();

	if(!hasEnded())
	{
		for each(Unit unit in UnitTracker::Instance().selectAllUnits())
		{
			if(!unit->isCompleted())
				continue;

			if(mUnits.count(unit) != 0)
				continue;

			const BWAPI::UnitType &type = unit->getType();
			if(type.isBuilding())
				continue;
			if(type.isAddon())
				continue;
			if(type == BWAPI::Broodwar->self()->getRace().getSupplyProvider())
				continue;
			if(type == BWAPI::UnitTypes::Zerg_Egg)
				continue;
			if(type == BWAPI::UnitTypes::Protoss_Interceptor)
				continue;
			if(type == BWAPI::UnitTypes::Zerg_Larva)
				continue;
			if(type == BWAPI::UnitTypes::Protoss_Scarab)
				continue;
			if(type == BWAPI::UnitTypes::Protoss_Observer)
				continue;

			RequirementGroup req;

			req.addUnitRequirement(unit, type.isWorker() ? 5 : 20, Requirement::maxTime);

			addRequirement(req);
		}

		if(!mObserver)
		{
			RequirementGroup req;

			req.addUnitFilterRequirement(20, Requirement::maxTime, UnitFilter(BWAPI::UnitTypes::Protoss_Observer) && UnitFilter(UnitFilterFlags::IsComplete));

			addRequirement(req);
		}
	}
}