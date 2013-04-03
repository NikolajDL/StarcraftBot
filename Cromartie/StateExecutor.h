#pragma once
#include "GA/State.h"

class StateExecutor
{
private:
	int currentGene;
	State currentState;
public:
	StateExecutor(void);
	~StateExecutor(void);
	void SetState(const State state);
	void executeNextGene();
	void unitComplete(BWAPI::UnitType unit);
};

