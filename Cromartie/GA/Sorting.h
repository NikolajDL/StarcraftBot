#pragma once
#include <vector>
#include "Chromosome.h"

class Sorting
{
public:
	Sorting(void);
	~Sorting(void);
	static void sort(std::vector<Chromosome>& pop);
};

