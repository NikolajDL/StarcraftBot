#pragma once

#include "Interface.h"

#include "BaseSquad.h"
#include "Behaviour.h"
#include "BorderTracker.h"

class AttackSquadTask : public BaseSquadTask
{
public:
	AttackSquadTask(ArmyBehaviour behaviour);

	virtual int getEndTime() const;
	virtual int getEndTime(Unit unit) const;

	virtual int getPriority(Unit unit) const;

	virtual Position getStartLocation(Unit unit) const;
	virtual Position getEndLocation(Unit unit) const;

	virtual bool preUpdate();
	virtual bool update();

	virtual bool waitingForUnit(Unit unit) const;
	virtual void giveUnit(Unit unit);
	virtual void returnUnit(Unit unit);
	virtual bool morph(Unit unit, BWAPI::UnitType previousType);
	virtual UnitGroup getFinishedUnits();

	virtual std::string getTaskName() const { return "Default Squad"; }
	virtual std::string getOutputName() const { return ""; }

	unsigned int controlSize() { return mUnits.size(); }

	virtual void updateRequirements();

	virtual void attack() { mIsAttacking = true; };
	virtual void stop() { mIsAttacking = false; };

	virtual const UnitGroup units() const { return mUnits; }

private:
	bool mIsAttacking;
	UnitGroup mUnits;
	UnitGroup mSavedUnits;
	std::map<Unit, Behaviour> mUnitBehaviours;
	ArmyBehaviour mCurrentBehaviour;

	BorderPosition getLargestChoke(const std::set<BorderPosition> &chokes);

	Unit mObserver;
	Goal mLastGoal;
};
typedef std::tr1::shared_ptr<AttackSquadTask> AttackSquadPointer;