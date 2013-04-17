#include "Stats.h"
#include "GA/Sorting.h"
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <fstream>
#include "ScoreHelper.h"

Stats::Stats(void)
{
}


Stats::~Stats(void)
{
}


void Stats::logPop(vector<Chromosome> pop, int elabsedTime, bool winner)
{
	Sorting::sort(pop);

	double bestFitness = pop.at(0).getFitness();
	double avgFitness = 0;
	int count = 0;

	for (int i = 0; i < pop.size(); i++)
	{
		if (pop.at(i).getFitness() != -999)
		{
			avgFitness += pop.at(i).getFitness();
			count++;
		}
	}

	avgFitness = avgFitness / (double) count;

	writeToFile(bestFitness, avgFitness, elabsedTime, ScoreHelper::getUnitScore(), ScoreHelper::getKillScore(), ScoreHelper::getBuildingScore(), winner);
}

void Stats::writeToFile(double best, double avg, int elabsedTime, int unitScore, int killScore, int buildingScore, bool winner)
{
	std::string bestStr = boost::lexical_cast<std::string>(best);
	std::string avgStr = boost::lexical_cast<std::string>(avg);
	std::string et = boost::lexical_cast<std::string>(elabsedTime);

	std::string us = boost::lexical_cast<std::string>(unitScore);
	std::string ks = boost::lexical_cast<std::string>(killScore);
	std::string bs = boost::lexical_cast<std::string>(buildingScore);

	 ofstream myfile;
	 myfile.open ("stats.txt", ios::app);
	 myfile << bestStr << ";" << avgStr << ";" << et << ";" << us << ";" << ks << ";" << bs << ";";
	 if (winner == true)
	 {
		 myfile << "winner";
	 }
	 else
	 {
		 myfile << "looser";
	 }
	 myfile << "\n";
	 myfile.close();
}