#include "StateExecutor.h"
#include "GA/BuildGene.h"
#include "GA/ResearchGene.h"
#include "GA/CombatGene.h"
#include "GA/AttackGene.h"
#include "EventManager.h"
#include "HypothalamusEvents.h"

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
		std::tr1::shared_ptr<Gene> g = state.getGenes().at(i);

		if(typeid(*g) != typeid(BuildGene))
		{
			BuildGene bg = dynamic_cast<BuildGene&>(*g);

			EQUEUE(new BuildUnitEvent(&bg.getBuildingType()));
		}
		else if (typeid(*g) == typeid(ResearchGene))
		{
			ResearchGene rg = dynamic_cast<ResearchGene&>(*g);
		}
		else if(typeid(*g) == typeid(AttackGene))
		{
			AttackGene ag = dynamic_cast<AttackGene&>(*g);
		}
		else if(typeid(*g) == typeid(CombatGene))
		{
			CombatGene cg = dynamic_cast<CombatGene&>(*g);

			for (int j = 0; j < cg.getAmount(); j++)
			{
				EQUEUE(new BuildUnitEvent(&cg.getUnitType()));
			}
		}
	}
}