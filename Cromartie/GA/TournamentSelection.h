#pragma once
#include <vector>
#include "Chromosome.h"

class TournamentSelection
{
public:
	TournamentSelection(void);
	~TournamentSelection(void);
	void selectAndMutate(std::vector<Chromosome>& population);
}; 

