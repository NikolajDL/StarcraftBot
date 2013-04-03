#pragma once
#include "GA/State.h"

class StateExecutor
{
public:
	StateExecutor(void);
	~StateExecutor(void);
	void ExecuteState(const State state);
};

