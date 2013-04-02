#pragma once
#include <vector>
#include "State.h"

class Chromosome
{
private:
	std::vector<State> states;
	double fitness;
	int nextStateIndex;
public:
	Chromosome(void);
	Chromosome(std::vector<State> s);
	~Chromosome(void);
	int temp;
	State getNextState();
	void setFitness(double value);
	const std::vector<State>& getStates(void) const;
	void addState(State s);
	int getFitness(void) const;
};

