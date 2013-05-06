#pragma once
#include <vector>
#include "State.h"
#include "../DbElement.h"

class Chromosome : public DbElement
{
private:
	std::vector<State> states;
	double fitness;
public:
	Chromosome(void);
	Chromosome(std::vector<State> s);
	~Chromosome(void);
	int temp;
	double prop;
	void setFitness(double value);
	std::vector<State> getStates(void) const;
	State& getState(int index);
	void addState(State s);
	double getFitness(void) const;
};

