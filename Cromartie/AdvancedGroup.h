#pragma once

#include "Interface.h"

#include "Goal.h"
#include "Behaviour.h"

class AdvancedGroup
{
public:
	AdvancedGroup();

	void update(Goal goal);

	void addUnit(Unit unit);
	void removeUnits(Unit unit);
	
	const UnitGroup units() const { return mAllUnits; }
private:
	UnitGroup mAllUnits;
	std::map<Unit, Behaviour> mUnitBehaviours;

	Goal mLastGoal;
	UnitGroup mCurrectGoalGroup;

	UnitGroup mReinforcements;

	std::list<std::pair<UnitGroup, Goal>> mStrandedUnits;
};