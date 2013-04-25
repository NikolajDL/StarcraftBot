#include "TournamentSelection.h"
#include "Sorting.h"
#include "GeneticOperator.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "../Settings.h"

extern boost::random::mt19937 randomGen;

TournamentSelection::TournamentSelection(void)
{
}


TournamentSelection::~TournamentSelection(void)
{
}

void TournamentSelection::selectAndMutate(std::vector<Chromosome>& population)
{
	boost::random::uniform_int_distribution<> dist(0, population.size()-1);
	
	std::vector<int> sampleIndex;
	std::vector<Chromosome> sample;

	if (population.size() < SAMPLE_SIZE)
	{
		std::cout << "Sample size bigger than pop size. This is not allowed\n";
		return;
	}

	// We take a random sample of size sampleSize from the population
	while (sample.size() < SAMPLE_SIZE)
	{
		int random = dist(randomGen);
		Chromosome& randomChromosome = population.at(random);
		// temp is used to store the index of the chromosome in population
		randomChromosome.temp = random;
		// Checks if chossenIndex already contains the chromosome
		bool alreadyInSample = false;

		for (size_t i = 0; i < sampleIndex.size(); i++)
		{
			if (random == sampleIndex.at(i))
				alreadyInSample = true;
		}
		if(alreadyInSample == false)
		{
			sampleIndex.push_back(random);
			sample.push_back(randomChromosome);
		}
		else
		{
			// The chromosome already exists in the sample, so no add
		}
	}

	// Sorts the sample according to fitness value. We do this so that we can pick out the best from the sample
	Sorting::sort(sample);
	
	std::vector<Chromosome> winners;
	std::vector<Chromosome> loosers;

	// We add the best chromosomes from the sample to winners
	for (int i = 0; i < WINNING_SIZE; i++)
	{
		winners.push_back(sample.at(i));
	}

	// We add the worst chromosome from the sample to the loosers
	for (int i = 0; i < LOOSING_SIZE; i++)
	{
		loosers.push_back(sample.at(sample.size() - 1 - i));
	}


	
	for (size_t i =0; i < loosers.size(); i++)
	{
		for (size_t j = 0; i < population.size(); j++)
		{
			if (population.at(j).temp == loosers.at(i).temp)
			{
				population.erase(population.begin() + j);
				break;
			}
		}
		
	}



	// For each winner we apply a genetic operator
	for (size_t i = 0; i < winners.size(); i++)
	{
		boost::random::uniform_int_distribution<> dist2(0, 9);
		int random = dist(randomGen);

		if (random > 6) // 30% chance
		{
			 Chromosome child = GeneticOperator::RuleReplaceMutation(winners.at(i));
			 population.push_back(child);
			 
		}
		else if (random < 7 && random > 3) // 30% chance
		{
			Chromosome child = GeneticOperator::RuleBiasedMutation(winners.at(i));
			population.push_back(child);
		}
		else if (random < 4 && random > 0) // 30% chance
		{
			bool didFindThreeMatches = false;
			for (size_t j = 0; j < population.size(); j++)
			{
				bool threeMatchesFound;
				Chromosome child = GeneticOperator::StateCrossover(winners.at(i), population.at(0), threeMatchesFound);
				if (threeMatchesFound == true)
				{
					population.push_back(child);
					didFindThreeMatches = true;
					break;
				}
			}
			if (didFindThreeMatches == false)
			{
				i--;
			}
			if (didFindThreeMatches)
				std::cout<<"Never found three state matches for State Crossover opperation\n";
			else
				std::cout<<"Found thre state matches for state crossover. This is very good\n";
		}
		else if (random == 0) // 10% chance
		{
			Chromosome child = GeneticOperator::RandomChromosome();
			population.push_back(child);
		}
	}


}

