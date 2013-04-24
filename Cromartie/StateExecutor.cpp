#include "StateExecutor.h"
#include "GA/BuildGene.h"
#include "GA/ResearchGene.h"
#include "GA/CombatGene.h"
#include "GA/AttackGene.h"
#include "EventManager.h"
#include "HypothalamusEvents.h"
#include <string>

StateExecutor::StateExecutor(void) : assimilatorCount(0)
{
}


StateExecutor::~StateExecutor(void)
{
}

bool StateExecutor::executeState(const State& state)
{
	//currentState = state;

	for (int i = 0; i < state.getGenes().size();i ++)
	{
		std::tr1::shared_ptr<Gene> g = state.getGenes().at(i);

		if(typeid(*g) == typeid(BuildGene))
		{
			BuildGene bg = dynamic_cast<BuildGene&>(*g);

			std::string name = bg.getBuildingType().getName();

			BWAPI::UnitType unit = bg.getBuildingType();

			BWAPI::Broodwar->sendText(name.c_str());
			
			if (unit == BWAPI::UnitTypes::Protoss_Nexus)
			{
				EQUEUE(new BuildUnitEvent(bg.getBuildingType(), TaskType::BuildOrder, 1, BuildingLocation::Expansion));
			}
			else if (unit == BWAPI::UnitTypes::Protoss_Assimilator)
			{
				if (assimilatorCount == 0)
				{
					assimilatorCount++;
					EQUEUE(new BuildUnitEvent(bg.getBuildingType(), TaskType::BuildOrder, 1, BuildingLocation::ExpansionGas));
				}
				else
				{
					return false;
				}
			}
			else
			{
				EQUEUE(new BuildUnitEvent(bg.getBuildingType(), TaskType::BuildOrder));
			}
		}
		else if (typeid(*g) == typeid(ResearchGene))
		{
			ResearchGene rg = dynamic_cast<ResearchGene&>(*g);
			
			if(rg.getUpgradeType() != BWAPI::UpgradeTypes::None)
			{
				EQUEUE(new UpgradeEvent(rg.getUpgradeType(), 1));
				BWAPI::Broodwar->sendText(rg.getUpgradeType().getName().c_str());
			}
			else if(rg.getTechType() != BWAPI::TechTypes::None)
			{
				EQUEUE(new UpgradeEvent(rg.getTechType()));
				BWAPI::Broodwar->sendText(rg.getUpgradeType().getName().c_str());
			}
			else
				std::cout << "StateExecutor::executeState(): Unable to determine upgrade type" << std::endl;
		}
		else if(typeid(*g) == typeid(AttackGene))
		{
			AttackGene ag = dynamic_cast<AttackGene&>(*g);
			if(ag.getAttack())
				EQUEUE(new AttackEvent());
		}
		else if(typeid(*g) == typeid(CombatGene))
		{
			CombatGene cg = dynamic_cast<CombatGene&>(*g);

			for (int j = 0; j < cg.getAmount(); j++)
			{
				BWAPI::Broodwar->sendText(cg.getUnitType().getName().c_str());
				EQUEUE(new BuildUnitEvent(cg.getUnitType()));
			}
		}
	}
	return true;
}