#pragma once
#include "GA/Chromosome.h"

class Stats
{
public:
	Stats(void);
	~Stats(void);
	static void logPop(vector<Chromosome> pop);
	static void writeToFile(double best, double avg);
};

