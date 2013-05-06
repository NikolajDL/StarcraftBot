#include "RouletteSelection.h"
#include "GA/GeneticOperator.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "Settings.h"
#include "GA/Sorting.h"

extern boost::random::mt19937 randomGen;

RouletteSelection::RouletteSelection(void)
{
}


RouletteSelection::~RouletteSelection(void)
{
}

void RouletteSelection::selectAndMutate(std::vector<Chromosome>& population)
{
	// Removes the worst chromsomes from the population
	Sorting::sort(population);

	for (int i = 0; i < population.size(); i++)
	{
		population.at(i).temp = i;
	}

	std::vector<Chromosome> loosers;

	// We add the worst chromosome from the sample to the loosers
	for (int i = 0; i < LOOSING_SIZE; i++)
	{
		loosers.push_back(population.at(population.size() - 1 - i));
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




	boost::random::uniform_real_distribution<> dist(0, 1);
	
	double totalFitness = 0;

	for (int i = 0; i < population.size(); i++)
	{
		totalFitness += population.at(i).getFitness();
	}

	double sumProp = 0;

	for (int i = 0; i < population.size(); i++)
	{
		double prop = sumProp + (population.at(i).getFitness() / totalFitness);
		population.at(i).prop = prop;
		sumProp += prop;
	}

	//std::vector<int> sampleIndex;
	std::vector<Chromosome> winners;

	for (int i = 0; i < WINNING_SIZE;i ++)
	{
		double random = dist(randomGen);
		for (int j = 0; j < population.size(); j++)
		{
			if (j != 0)
			{
				if (random < population.at(j).prop && random > population.at(j-1).prop)
				{
					winners.push_back(population.at(j));
					break;
				}
			}
			else
			{
				if (random < population.at(j).prop)
				{
					winners.push_back(population.at(j));
					break;
				}
			}
		}
	}

		// For each winner we apply a genetic operator
	for (size_t i = 0; i < winners.size(); i++)
	{
		boost::random::uniform_int_distribution<> dist2(0, 9);
		int random = dist2(randomGen);

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
			bool threeMatchesFound = false;
			for(size_t j=0;j<winners.size(); j++)
			{
				if(j==i) continue;
				Chromosome child = GeneticOperator::StateCrossover(winners.at(i), winners.at(j), threeMatchesFound);
				if(threeMatchesFound) 
				{
					population.push_back(child);
					break;
				}
			}
			// If the StateCrossover operator was unsuccesful, keep population size constant
			if(!threeMatchesFound)
				population.push_back(GeneticOperator::RandomChromosome());
		}
		else if (random == 0) // 10% chance
		{
			Chromosome child = GeneticOperator::RandomChromosome();
			population.push_back(child);
		}
	}

//	for all members of population
//    sum += fitness of this individual
//end for
//
//for all members of population
//    probability = sum of probabilities + (fitness / sum)
//    sum of probabilities += probability
//end for
//
//loop until new population is full
//     do this twice
//         number = Random between 0 and 1
//       for all members of population
//           if number > probability but less than next probability 
//                then you have been selected
//       end for
//     end
//     create offspring
//end loop
}

//
//template <typename N> N getRandom(N min, N max)
//{
//  timeval t;
//  gettimeofday(&t,NULL);
//  boost::mt19937 seed( (int)t.tv_sec );
//  boost::uni dist(min,max);
//  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > random(seed,dist);
//  return random(); 
//}