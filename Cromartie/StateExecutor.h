#pragma once
#include "GA/State.h"

typedef std::map<BWAPI::UpgradeType,int> UpgradeMap;

class StateExecutor
{
private:
	UpgradeMap upgradeLevels;
	int getUpgradeLevel(BWAPI::UpgradeType up);
public:
	StateExecutor(void);
	~StateExecutor(void);
	bool executeState(const State& state);
};


