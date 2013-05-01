#include "StateExecutor.h"
#include "GA/BuildGene.h"
#include "GA/ResearchGene.h"
#include "GA/CombatGene.h"
#include "GA/AttackGene.h"
#include "EventManager.h"
#include "HypothalamusEvents.h"
#include "BaseTracker.h"
#include <string>

StateExecutor::StateExecutor(void) : assimilatorCount(0), nexusCount(1)
{
}


StateExecutor::~StateExecutor(void)
{
}

int StateExecutor::getUpgradeLevel(BWAPI::UpgradeType up)
{
	std::pair<UpgradeMap::iterator, bool> ins = this->upgradeLevels.insert(std::make_pair(up, 1));
	
	if ( ! ins.second ) {
		// Key already exists
		return ++((ins.first)->second);
	} else {
		// Key is new, and was created
		return 1;
	}
}

bool StateExecutor::executeState(const State& state)
{
	for (size_t i = 0; i < state.getGenes().size();i ++)
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
				this->nexusCount++;
			}
			else if (unit == BWAPI::UnitTypes::Protoss_Assimilator)
			{
				// If the game has JUST started, the base tracker has not yet analyzed the map and thus we have 0 bases
				if(BWAPI::Broodwar->getFrameCount() > 0)
				{
					// Tempting to use Base.getGeysers() but we cannot, due to the scenario of
					// 1 assimilator has just been ordered to be built. The probe is moving towards the geyser,
					// but has not yet built the assimilator. Then another assimilator gene is made but is ignored, 
					// as getGeyser() returns 1 (the geyser does not have an assimilator yet). This means no building is under
					// costruction, and the bot stalls forever in this state.
					if(nexusCount > assimilatorCount)
					{
						EQUEUE(new BuildUnitEvent(bg.getBuildingType(), TaskType::BuildOrder, 1, BuildingLocation::ExpansionGas));
						assimilatorCount++;
					}
					else
					{
						return false;
					}
				}
				else
				{
					EQUEUE(new BuildUnitEvent(bg.getBuildingType(), TaskType::BuildOrder, 1, BuildingLocation::ExpansionGas));
					assimilatorCount++;
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
				EQUEUE(new UpgradeEvent(rg.getUpgradeType(), getUpgradeLevel(rg.getUpgradeType())));
			}
			else if(rg.getTechType() != BWAPI::TechTypes::None)
			{
				EQUEUE(new UpgradeEvent(rg.getTechType()));
			}
			else
				BWAPI::Broodwar->sendText("StateExecutor::executeState(): Unable to determine upgrade type");
		}
		else if(typeid(*g) == typeid(AttackGene))
		{
			AttackGene ag = dynamic_cast<AttackGene&>(*g);
			if(ag.getAttack())
			{
				EQUEUE(new AttackEvent());
				BWAPI::Broodwar->sendText("Attack!");
			}
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