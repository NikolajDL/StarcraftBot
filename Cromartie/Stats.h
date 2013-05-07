#pragma once
#include "GA/Chromosome.h"

class Stats
{
public:
	static void logPop(std::vector<Chromosome> pop);
	static void writeToFile(double best, double avgint);
	static void logInidivdualGame(bool winner, int elabsedTime, int unitScore, int killScore, int buildingScore);
};

