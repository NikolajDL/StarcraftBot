#include "Chromosome.h"


Chromosome::Chromosome(void) : fitness(-999), nextStateIndex(0)
{
}

Chromosome::Chromosome(std::vector<State> s) : fitness(-999), nextStateIndex(0), states(s)
{
}

Chromosome::~Chromosome(void)
{
}

State Chromosome::getNextState(void)
{
	State s = states.at(this->nextStateIndex++);

	return s;
}

int Chromosome::getFitness(void) const
{
	return fitness;
}

const std::vector<State>& Chromosome::getStates(void) const 
{
	return this->states;
}

void Chromosome::addState(State s)
{
	this->states.push_back(s);
}

void Chromosome::setFitness(double value) 
{
	this->fitness = value;
}