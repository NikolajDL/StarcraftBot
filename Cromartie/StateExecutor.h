#pragma once
#include "GA/State.h"

class StateExecutor
{
private:
	int currentGene;
	State currentState;
	std::vector<BWAPI::UnitType> enquedUnits;
public:
	StateExecutor(void);
	~StateExecutor(void);
	void SetState(const State state);
	void executeNextGene();
	bool isStateFinish();
	void unitComplete(BWAPI::UnitType unit);
};

