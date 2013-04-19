#pragma once
#include "Chromosome.h"

class GeneticOperator
{
public:

	static Chromosome StateCrossover(const Chromosome& parent1, const Chromosome& parent2, bool& threeMatchesFound);
	static Chromosome RuleReplaceMutation(const Chromosome& parent);
	static Chromosome RuleBiasedMutation(const Chromosome& parent);
	static Chromosome RandomChromosome(void);

};

