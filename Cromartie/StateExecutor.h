#pragma once
#include "GA/State.h"

class StateExecutor
{
private:
	int currentGene;
	State currentState;
	std::vector<BWAPI::UnitType> enquedUnits;
	int assimilatorCount;
public:
	StateExecutor(void);
	~StateExecutor(void);
	bool executeState(const State state);
	//void executeNextGene();
	//bool isStateFinish();
	//void unitComplete(BWAPI::UnitType unit);
};

