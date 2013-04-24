#pragma once
#include <vector>
#include "GA/Chromosome.h"


class RouletteSelection
{
public:
	RouletteSelection(void);
	~RouletteSelection(void);
	void selectAndMutate(std::vector<Chromosome>& population);
};

