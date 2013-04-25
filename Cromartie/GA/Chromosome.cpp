#include "Chromosome.h"


Chromosome::Chromosome(void) : fitness(-999)
{
}

Chromosome::Chromosome(std::vector<State> s) : fitness(-999), states(s)
{
}

Chromosome::~Chromosome(void)
{
}

double Chromosome::getFitness(void) const
{
	return fitness;
}

std::vector<State> Chromosome::getStates(void) const
{
	return this->states;
}

State& Chromosome::getState(int index)
{
	return this->states.at(index);
}

void Chromosome::addState(State s)
{
	this->states.push_back(s);
}

void Chromosome::setFitness(double value) 
{
	this->fitness = value;
}