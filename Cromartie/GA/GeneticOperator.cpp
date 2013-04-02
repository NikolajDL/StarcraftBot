#include "GeneticOperator.h"
#include "UnitType.h"
#include "BuildGene.h"
#include "AttackGene.h"
#include "ResearchGene.h"
#include "CombatGene.h"
#include "State.h"
#include "StarcraftRules.h"

#include <stdlib.h>
#include <time.h>
#include <typeinfo>
#include <algorithm>
#include <iterator>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
boost::random::mt19937 randomGen;

// Select 2 parents and check if the selected parents have at 
// least 3 matching activated states for crossover. We make sure that 
// the child chromosome inherits genetic material from both parents 
// to prevent a parent from being copied completely onto the child chromosome. 
// Between two matching states, all states and genes are copied from either parent
Chromosome GeneticOperator::StateCrossover(const Chromosome& parent1, const Chromosome& parent2, bool* threeMatchesFound)
{
	std::vector<State> childStates;
	bool takeFromParent1 = true;
	int lastMatch = 0;
	int matches = 0;
	int stateSize = std::min(parent1.getStates().size(),parent2.getStates().size());

	// Counts the number of matches. At least three is required
	for(int i=0;i<stateSize;i++)
	{
		if(parent1.getStates().at(i) == parent2.getStates().at(i))
		{
			matches++;
		}
	}

	if (matches < 3)
	{
		*threeMatchesFound = false;
		return Chromosome(childStates);
	}
	else
		*threeMatchesFound = true;

	for(int i=0;i<stateSize;i++)
	{
		if(parent1.getStates().at(i) == parent2.getStates().at(i))
		{
			if(takeFromParent1)
			{
				std::copy(parent1.getStates().begin()+lastMatch, parent1.getStates().begin()+i,std::back_inserter(childStates));
			}
			else
			{
				std::copy(parent2.getStates().begin()+lastMatch, parent2.getStates().begin()+i,std::back_inserter(childStates));
			}
			lastMatch = i;
		}
	}

	// Add remaining states after last matching state
	if(takeFromParent1)
	{
		std::copy(parent1.getStates().begin()+lastMatch, parent1.getStates().end(),std::back_inserter(childStates));
	}
	else
	{
		std::copy(parent2.getStates().begin()+lastMatch, parent2.getStates().end(),std::back_inserter(childStates));
	}

	return Chromosome(childStates);
}

// Select 1 parent and for every activated state all
// economy, research or combat rules have a 25% chance of being replaced.
// Building rules are excluded here both for replacement and as replacement,
// because these could spawn a state change and could possibly corrupt the
// chromosome. Genes in inactivated states are ignored as they are considered
// ‘dead’ and mutation doesn’t really make sense here. 
Chromosome GeneticOperator::RuleReplaceMutation(const Chromosome& parent)
{
	std::vector<State> childStates = parent.getStates();


	// TODO: Only select activated states
	for(int i=0;i<childStates.size();i++) {
		State& s = childStates.at(i);
		for (int j=0;j<s.getGenes().size(); j++){
			const std::tr1::shared_ptr<Gene> g = s.getGenes().at(j);
			if(typeid(*g) != typeid(BuildGene))
			{				 
				boost::random::uniform_int_distribution<> dist(1, 100);
				int randomNr = dist(randomGen);
				if(randomNr <= 25)
				{
					boost::random::uniform_int_distribution<> dist2(0, 1);
					int replaceNr = dist2(randomGen);
					if(typeid(*g) == typeid(CombatGene))
					{
						if (replaceNr == 1)
						{
							bool found;
							std::tr1::shared_ptr<ResearchGene> gene = StarcraftRules::getValidResearchGene( s, found);
							if (found == true)
								s.replaceGeneAt(j,gene);
							else
								std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
						}
						else
						{
							bool found;
							std::tr1::shared_ptr<AttackGene> gene = StarcraftRules::getValidAttackGene( s, found);
							if (found == true)
								s.replaceGeneAt(j,gene);
							else
								std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
						}
					}
					else if(typeid(*g) == typeid(ResearchGene))
					{
						if (replaceNr == 1)
						{
							bool found;
							std::tr1::shared_ptr<AttackGene> gene = StarcraftRules::getValidAttackGene( s, found);
							if (found == true)
								s.replaceGeneAt(j,gene);
							else
								std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
						}
						else
						{
							bool found;
							std::tr1::shared_ptr<CombatGene> gene = StarcraftRules::getValidCombatGene( s, found);
							if (found == true)
								s.replaceGeneAt(j,gene);
							else
								std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
						}
					}
					else if(typeid(*g) == typeid(AttackGene))
					{
						if (replaceNr == 1)
						{
							bool found;
							std::tr1::shared_ptr<ResearchGene> gene = StarcraftRules::getValidResearchGene( s, found);
							if (found == true)
								s.replaceGeneAt(j,gene);
							else
								std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
						}
						else
						{
							bool found;
							std::tr1::shared_ptr<CombatGene> gene = StarcraftRules::getValidCombatGene( s, found);
							if (found == true)
								s.replaceGeneAt(j,gene);
							else
								std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
						}
					}

				}
			}
		
		}
	};

	return Chromosome(childStates);
}

// Select 1 parent and for every activated state the
// parameters for existing combat or economy rules have 
// a 50% chance of being mutated. The mutation is within a 
// predefined boundary (between a minimum and maximum value). 
// For this genetic operator, we exclude build and research rules.
Chromosome GeneticOperator::RuleBiasedMutation(const Chromosome& parent)
{
	std::vector<State> childStates = parent.getStates();
	for(int i=0;i<childStates.size();i++) {
		State& s = childStates.at(i);
		for (int j=0;j<s.getGenes().size(); j++){
			const std::tr1::shared_ptr<Gene> g = s.getGenes().at(j);
			if(typeid(*g) != typeid(BuildGene) && typeid(*g) != typeid(ResearchGene))
			{				 
				// Mutate gene
				boost::random::uniform_int_distribution<> dist(0, 1);
				int randomNr = dist(randomGen);
				if(randomNr)
				{
					if(typeid(*g) != typeid(AttackGene))
					{
						// Mutate parameters of gene
						bool found;
						std::tr1::shared_ptr<AttackGene> gene = StarcraftRules::getValidAttackGene(s, found);
						if (found)
							s.replaceGeneAt(j, gene);
						else
							std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
					}
					else if(typeid(*g) != typeid(CombatGene))
					{
						// Mutate parameters of gene
						bool found;
						std::tr1::shared_ptr<CombatGene> gene = StarcraftRules::getValidCombatGene(s, found);
						if (found)
							s.replaceGeneAt(j, gene);
						else
							std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
					}
				}
			}
		}
	}

	return Chromosome(childStates);
}

#define CHROMOSOME_LENGTH 50

// Generate a complete new chromosome
Chromosome GeneticOperator::RandomChromosome(void)
{
	
	boost::random::uniform_int_distribution<> dist(1, 4);
	int randomNrGene = dist(randomGen);

	Chromosome c;
	std::vector<UnitType> buildSequence;
	
	for(int i=0; i<=CHROMOSOME_LENGTH; i++)
	{
		State s(buildSequence);
		while(randomNrGene != 4) // 4 = build gene
		{
			bool found;
			switch(randomNrGene)
			{
				case 1:	
					{
						std::tr1::shared_ptr<AttackGene> gene = StarcraftRules::getValidAttackGene(s, found);
						if (found == true)
							s.addGene(gene);
						else
							std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
					}
					break;
				case 2:
					{
						std::tr1::shared_ptr<CombatGene> gene = StarcraftRules::getValidCombatGene(s, found);
						if (found == true)
							s.addGene(gene);
						else
							std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
					}
					break;
				case 3:	
					{
						std::tr1::shared_ptr<ResearchGene> gene = StarcraftRules::getValidResearchGene(s, found);
						if (found == true)
							s.addGene(gene);
						else
							std::cout << "State::addGene(): Trying to add NULL gene, ignoring it\n";
					}
					break;
			}
			randomNrGene = dist(randomGen);
		}
		s.addGene(StarcraftRules::getValidBuildGene(s));
		c.addState(s);

		buildSequence = s.getBuildingSequence();
		randomNrGene = dist(randomGen);
	}

	return c;
}


