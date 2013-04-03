#pragma once
#include <vector>
#include <memory>
#include "../Interface.h"
#include "Gene.h"

using namespace std;

class State
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
inline bool operator==(const State& a, const State& b)
{
	bool match = true;
	// TODO: Fix this
	int seqSize = min(a.getBuildingSequence().size(), b.getBuildingSequence().size());
	for(int j=0; j<seqSize; j++)
	{
		if(a.getBuildingSequence().at(j).getName() != b.getBuildingSequence().at(j).getName())
		{
			match = false;
		}
	}
	return match;
}

