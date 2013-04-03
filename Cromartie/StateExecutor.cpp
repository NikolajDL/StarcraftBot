#include "StateExecutor.h"
#include "GA/BuildGene.h"
#include "GA/ResearchGene.h"
#include "GA/CombatGene.h"
#include "GA/AttackGene.h"

StateExecutor::StateExecutor(void)
{
}


StateExecutor::~StateExecutor(void)
{
}


void StateExecutor::ExecuteState(const State state)
{
	for (int i = 0; i < state.getGenes().size();i ++)
	{
		std::tr1::shared_ptr<Gene> g = state.getGenes().at(j);

		if(typeid(*g) != typeid(BuildGene))
		{
		}
		else if (typeid(*g) == typeid(ResearchGene))
		{
		}
		else if(typeid(*g) == typeid(AttackGene))
		{
		}
		else if(typeid(*g) == typeid(CombatGene))
		{
		}
	}
}