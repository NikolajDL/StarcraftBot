#pragma once
#include "GA/Chromosome.h"

class Stats
{
public:
	Stats(void);
	~Stats(void);
	static void logPop(vector<Chromosome> pop, int elabsedTime, bool winner);
	static void writeToFile(double best, double avgint, int elabsedTime, int unitScore, int killScore, int buildingScore, bool winner);
};

