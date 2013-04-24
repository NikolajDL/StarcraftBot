#pragma once
#include "GA/State.h"


class StateExecutor
{
private:
	int assimilatorCount;
	std::vector<std::pair<BWAPI::UpgradeType,int>> upgradeLevels;
	int getUpgradeLevel(BWAPI::UpgradeType up);
public:
	StateExecutor(void);
	~StateExecutor(void);
	bool executeState(const State& state);
};

