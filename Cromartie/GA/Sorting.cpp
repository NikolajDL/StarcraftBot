#include "Sorting.h"


Sorting::Sorting(void)
{
}


Sorting::~Sorting(void)
{
}


void Sorting::sort(std::vector<Chromosome>& pop)
{
	std::vector<Chromosome> sorted;

	while (pop.size() != 0)
	{
		double bestFitness = -100000;
		Chromosome bestChromosome;
		int bestChromomeIndex = -1;

		for (int i = 0; i < pop.size(); i++)
		{
			if (pop.at(i).getFitness() > bestFitness)
			{
				bestChromosome = pop.at(i);
				bestFitness = bestChromosome.getFitness();
				bestChromomeIndex = i;
			}
		}
		sorted.push_back(bestChromosome);
		pop.erase(pop.begin() + bestChromomeIndex);
	}

	for (int i = 0; i < sorted.size(); i++)
	{
		pop.push_back(sorted.at(i));
	}
}