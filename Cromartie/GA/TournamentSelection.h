#pragma once
#include <vector>
#include "Chromosome.h"

class TournamentSelection
{
private:
	int sampleSize;
	int winningSize;
	int loosingSize;
public:
	TournamentSelection(void);
	~TournamentSelection(void);
	void selectAndMutate(std::vector<Chromosome>& population);
}; 

