#include "Stats.h"
#include "GA/Sorting.h"
#include "../boost/boost/lexical_cast.hpp"
#include <iostream>
#include <fstream>

Stats::Stats(void)
{
}


Stats::~Stats(void)
{
}


void Stats::logPop(vector<Chromosome> pop)
{
	Sorting::sort(pop);

	double bestFitness = pop.at(0).getFitness();
	double avgFitness = 0;

	for (int i = 0; i < pop.size(); i++)
	{
		if (pop.at(i).getFitness() != -999)
		{
			avgFitness += pop.at(i).getFitness();
		}
	}

	avgFitness = avgFitness / (double) pop.size();

	writeToFile(bestFitness, avgFitness);
}

void Stats::writeToFile(double best, double avg)
{
	std::string bestStr = boost::lexical_cast<std::string>(best);
	std::string avgStr = boost::lexical_cast<std::string>(best);

	 ofstream myfile;
	 myfile.open ("stats.txt", ios::app);
	 myfile << bestStr << ";" << avgStr << "\n";
	 myfile.close();
}