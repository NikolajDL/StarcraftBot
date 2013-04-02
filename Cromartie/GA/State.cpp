#include "State.h"
#include "AttackGene.h"
#include "CombatGene.h"
#include "ResearchGene.h"
#include "BuildGene.h"
#include "UnitType.h"
#include "StarcraftRules.h"
#include "Gene.h"
#include <typeinfo>
#include <iostream>


State::State(std::vector<BWAPI::UnitType> buildSequence) : buildingSequence(buildSequence), fitness(0)
{
}

State::~State(void)
{
}

std::vector<BWAPI::UnitType> State::getBuildingSequence(void) const
{
	return this->buildingSequence;
}

const std::vector<std::tr1::shared_ptr<Gene>>& State::getGenes(void) const
{
	return this->genes;
}

void State::addGene(std::tr1::shared_ptr<Gene> g)
{
	if(g == NULL)
	{
		std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
		return;
	}

	if(typeid(*g) == typeid(BuildGene))
	{
		this->buildingSequence.push_back(dynamic_cast<BuildGene&>(*g).getBuildingType());
	}
	this->genes.push_back(g);
}

double State::getFitness(void) const
{
	return this->fitness;
}

void State::setFitness(double value)
{
	this->fitness = value;
}

void State::replaceGeneAt(int at, std::tr1::shared_ptr<Gene> gene)
{
	this->genes.at(at) = gene;
}