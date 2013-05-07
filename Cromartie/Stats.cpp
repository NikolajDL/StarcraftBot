#include "Stats.h"
#include "GA/Sorting.h"
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <fstream>
#include "ScoreHelper.h"

void Stats::logPop(std::vector<Chromosome> pop)
{
	Sorting::sort(pop);

	double bestFitness = pop.at(0).getFitness();
	double avgFitness = 0;
	int count = 0;

	for (size_t i = 0; i < pop.size(); i++)
	{
		if (pop.at(i).getFitness() != -999)
		{
			avgFitness += pop.at(i).getFitness();
			count++;
		}
	}

	avgFitness = avgFitness / (double) count;

	writeToFile(bestFitness, avgFitness);
}

void Stats::logInidivdualGame(bool winner, int elabsedTime, int unitScore, int killScore, int buildingScore)
{
	std::string et = boost::lexical_cast<std::string>(elabsedTime);

	std::string us = boost::lexical_cast<std::string>(unitScore);
	std::string ks = boost::lexical_cast<std::string>(killScore);
	std::string bs = boost::lexical_cast<std::string>(buildingScore);

	 std::ofstream myfile;
	 myfile.open ("statsForEachGame.txt", std::ios::app);
	 if (winner == true)
	 {
		 myfile << "winner";
	 }
	 else
	 {
		 myfile << "looser";
	 }
	 myfile << ";" << et << ";" << us << ";" << ks << ";" << bs << ";";
	 myfile << "\n";
	 myfile.close();
}

void Stats::writeToFile(double best, double avg)
{
	std::string bestStr = boost::lexical_cast<std::string>(best);
	std::string avgStr = boost::lexical_cast<std::string>(avg);

	 std::ofstream myfile;
	 myfile.open ("statsForEachGeneration.txt", std::ios::app);
	 myfile << bestStr << ";" << avgStr <<
	 myfile << "\n";
	 myfile.close();
}