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
	std::vector<Chromosome> sample;

	for (int i = 0; i < WINNING_SIZE;i ++)
	{
		double random = dist(randomGen);
		for (int j = 0; j < population.size(); j++)
		{
			if (j != 0)
			{
				if (random < population.at(j).prop && random > population.at(j-1).prop)
				{
					sample.push_back(population.at(j));
					break;
				}
			}
			else
			{
				if (random < population.at(j).prop)
				{
					sample.push_back(population.at(j));
					break;
				}
			}
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