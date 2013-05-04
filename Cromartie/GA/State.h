#pragma once
#include <vector>
#include <memory>
#include "../Interface.h"
#include "../DbElement.h"
#include "Gene.h"

class State : public DbElement
{
private:
	std::vector<BWAPI::UnitType> buildingSequence;
	std::vector<std::tr1::shared_ptr<Gene>> genes;
	double fitness;
public:
	State(void);
	State(std::vector<BWAPI::UnitType> buildSequence);
	~State(void);
	void addGene(std::tr1::shared_ptr<Gene> g);
	void replaceGeneAt(int at, std::tr1::shared_ptr<Gene> gene);
	double getFitness(void) const;
	void setFitness(double value);
	std::vector<BWAPI::UnitType> getBuildingSequence(void) const;
	const std::vector<std::tr1::shared_ptr<Gene>>& getGenes(void) const;
};

// Ignore genes
inline bool operator==(State& a, State& b)
{
	size_t seqSize = a.getBuildingSequence().size();
	if(a.getBuildingSequence().size() != b.getBuildingSequence().size() )
		return false;

	size_t matchCtr = 0;
	for(size_t i=0;i<seqSize;i++)
	{
		if(a.getBuildingSequence().at(i) == b.getBuildingSequence().at(i))
		{
			matchCtr++;
		}
	}

	if(matchCtr == seqSize)
		return true;
	else
		return false;
}

