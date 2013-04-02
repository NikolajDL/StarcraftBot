#include <iostream>

#include <vector>
#include <algorithm>

#include <iostream>
#include <vector>

#include "Chromosome.h"
#include "GeneticOperator.h"
#include "TournamentSelection.h"
#include "GA.h"
#include "Sorting.h"

#include "DatabaseManager.h"

int main()
{
	std::cout<<"Test";

	GA ga;
	Sorting sorting;
	TournamentSelection ts;


	std::vector<Chromosome> cs;

	for (int i = 0; i < 20; i++)
	{
		Chromosome c1 = GeneticOperator::RandomChromosome();
		c1.setFitness(i);
		cs.push_back(c1);
	}

	ts.selectAndMutate(cs);

	//ga.onStarcraftStart();
	
	//dm.createDB();
//	Chromosome c1 = GeneticOperator::RandomChromosome();
	//DatabaseManager dm;
	//dm.insertChromosome(c1);
	//dm.eraseDatabaseContent();
	


	
	//Chromosome c2 = GeneticOperator::RandomChromosome();
	//Chromosome c3 = GeneticOperator::RandomChromosome();

	//Chromosome test = GeneticOperator::StateCrossover(c1, c2);
	//test = GeneticOperator::RuleBiasedMutation(c1);
	//test = GeneticOperator::RuleReplaceMutation(c1);

	return 0;
}