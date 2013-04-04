#include "StateExecutor.h"
#include "GA/BuildGene.h"
#include "GA/ResearchGene.h"
#include "GA/CombatGene.h"
#include "GA/AttackGene.h"
#include "EventManager.h"
#include "HypothalamusEvents.h"
#include <string>

StateExecutor::StateExecutor(void)
{
}


StateExecutor::~StateExecutor(void)
{
}

//void StateExecutor::executeNextGene()
//{
//	std::tr1::shared_ptr<Gene> g = currentState.getGenes().at(currentGene);
//
//	enquedUnits.clear();
//	
//		if(typeid(*g) == typeid(BuildGene))
//		{
//			BuildGene bg = dynamic_cast<BuildGene&>(*g);
//
//			EQUEUE(new BuildUnitEvent(&bg.getBuildingType()));
//		}
//		else if (typeid(*g) == typeid(ResearchGene))
//		{
//			ResearchGene rg = dynamic_cast<ResearchGene&>(*g);
//		}
//		else if(typeid(*g) == typeid(AttackGene))
//		{
//			AttackGene ag = dynamic_cast<AttackGene&>(*g);
//		}
//		else if(typeid(*g) == typeid(CombatGene))
//		{
//			CombatGene cg = dynamic_cast<CombatGene&>(*g);
//
//			for (int j = 0; j < cg.getAmount(); j++)
//			{
//				enquedUnits.push_back(cg.getUnitType());
//				EQUEUE(new BuildUnitEvent(&cg.getUnitType()));
//			}
//		}
//
//	currentGene++;
//}

//bool StateExecutor::isStateFinish()
//{
//	if (currentGene == currentState.getGenes().size())
//		return true;
//	else
//		return false;
//}
//
//void StateExecutor::unitComplete(BWAPI::UnitType unit)
//{
//}

void StateExecutor::executeState(const State state)
{
	currentGene = 0;
	currentState = state;

	for (int i = 0; i < state.getGenes().size();i ++)
	{
		std::tr1::shared_ptr<Gene> g = state.getGenes().at(i);

		if(typeid(*g) == typeid(BuildGene))
		{
			BuildGene bg = dynamic_cast<BuildGene&>(*g);

			std::string name = bg.getBuildingType().getName();

			BWAPI::Broodwar->sendText(bg.getBuildingType().getName().c_str());

			BWAPI::UnitType unit = bg.getBuildingType();
			
			EQUEUE(new BuildUnitEvent(&BWAPI::UnitTypes::getUnitType(unit.getName()), 1));
		}
		else if (typeid(*g) == typeid(ResearchGene))
		{
			ResearchGene rg = dynamic_cast<ResearchGene&>(*g);

			EQUEUE(new UpgradeEvent(rg.getUpgradeType(), 3));
		}
		else if(typeid(*g) == typeid(AttackGene))
		{
			AttackGene ag = dynamic_cast<AttackGene&>(*g);
			EQUEUE(new AttackEvent());
		}
		else if(typeid(*g) == typeid(CombatGene))
		{
			CombatGene cg = dynamic_cast<CombatGene&>(*g);

			for (int j = 0; j < cg.getAmount(); j++)
			{
				BWAPI::Broodwar->sendText(cg.getUnitType().getName().c_str());
				EQUEUE(new BuildUnitEvent(&cg.getUnitType()));
			}
		}
	}
}