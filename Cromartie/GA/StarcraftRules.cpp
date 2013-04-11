#include "StarcraftRules.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
extern boost::random::mt19937 randomGen;
std::tr1::shared_ptr<CombatGene> StarcraftRules::getValidCombatGene(const State& s, bool& found)
{
	// Get all valid units
	std::vector<BWAPI::UnitType> validUnits = getValidUnits(s);

	// If not valid units exist, we return NULL
	if (validUnits.size() == 0)
	{
		std::tr1::shared_ptr<CombatGene> cg( new CombatGene() );
		found = false;
		return cg;
	}
	else
	{
		found = true;
	}

	// Choose a random unit
	boost::random::uniform_int_distribution<> dist(0, validUnits.size()-1);
	int randomNrGene = dist(randomGen);
	BWAPI::UnitType randomUnit= validUnits.at(randomNrGene);

	// Choose a random number of that unit
	boost::random::uniform_int_distribution<> dist2(1, 10);
	int randomAmount = dist2(randomGen);

	// Create a CombatGene and set the unit type and amount
	std::tr1::shared_ptr<CombatGene> cg( new CombatGene(randomUnit, randomAmount) );

	return cg;
}

std::tr1::shared_ptr<ResearchGene> StarcraftRules::getValidResearchGene(const State& s, bool& found)
{
	// Get all valid upgrades
	std::vector<BWAPI::UpgradeType> validUpgrades = getValidUpgrades(s);

	// If no valid upgrades exists, we return NULL
	if (validUpgrades.size() == 0)
	{
		std::tr1::shared_ptr<ResearchGene> rg( new ResearchGene() );
		found = false;
		return rg;
	}
	else
	{
		found = true;
	}

	// Choose a random upgrade type
	boost::random::uniform_int_distribution<> dist(0, validUpgrades.size()-1);
	int randomNrGene = dist(randomGen);
	BWAPI::UpgradeType randomUpgrade = validUpgrades.at(randomNrGene);

	// Create the research gene and assign the upgrade
	std::tr1::shared_ptr<ResearchGene> rg( new ResearchGene(randomUpgrade) );
	return rg;
}

std::tr1::shared_ptr<AttackGene> StarcraftRules::getValidAttackGene(const State& s, bool& found)
{
	// Choose a random number between 0 and 9
	boost::random::uniform_int_distribution<> dist(0, 9);
	int randomNrGene = dist(randomGen);

	// If 0, 1 or 2, create an attack gene
	if (randomNrGene < 3)
	{
		std::tr1::shared_ptr<AttackGene> ag( new AttackGene() );
		found = true;
		return ag;
	}
	// If not, do not create an attack gene
	else
	{
		std::tr1::shared_ptr<AttackGene> ag( new AttackGene() );
		found = false;
		return ag;
	}
}

std::tr1::shared_ptr<BuildGene> StarcraftRules::getValidBuildGene(const State& s)
{
	// Get all valid buildings
	std::vector<BWAPI::UnitType> validBuildings = getValidBuildings(s);

	// Choose a random building
	boost::random::uniform_int_distribution<> dist(0, validBuildings.size()-1);
	int randomNrGene = dist(randomGen);
	BWAPI::UnitType randomBuilding = validBuildings.at(randomNrGene);

	// Create the build gene and assign the random building to the gene
	std::tr1::shared_ptr<BuildGene> bg( new BuildGene(randomBuilding) );
	bg->name = randomBuilding.getName();

	return bg;
}

std::vector<BWAPI::UpgradeType> StarcraftRules::getValidUpgrades(const State& s)
{
	std::vector<BWAPI::UpgradeType> validUpgrades;

	// Loop through all buildings that has been build in order to determine which buildings are valid to build
	for (int i = 0; i < s.getBuildingSequence().size(); i++)
	{
		BWAPI::UnitType ut = s.getBuildingSequence().at(i);

		if (ut.getName() == "Protoss Forge")
		{
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Ground Weapons"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Ground Armor"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Plasma Shields"));
		}
		else if (ut.getName() == "Protoss Cybernetics Core")
		{
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Air Weapons"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Air Armor"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Singularity Charge"));
		}
		else if (ut.getName() == "Protoss Robotics Support Bay")
		{
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Increase Reaver Capacity"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Scarab Damage"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Gravitic Drive"));
		}
		else if (ut.getName() == "Protoss Observatory")
		{
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Gravitic Boosters"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Sensor Array"));
		}
		else if (ut.getName() == "Protoss Citadel Of Adun")
		{
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Leg Enhancements"));
		}
		else if (ut.getName() == "Protoss Templar Archives")
		{
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Hallucinations"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Psionic Storm"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Maelstrom"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Mind Control"));
		}
		else if (ut.getName() == "Protoss Arbiter Tribunal")
		{
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Khaydarin Core"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Recall"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Statis Field"));
		}
		else if (ut.getName() == "Protoss Fleet Beacon")
		{
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Disruption Web"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Apial Sensors"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Gravitic Thrusters"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Carrier Capacity"));
			validUpgrades.push_back(BWAPI::UpgradeTypes::getUpgradeType("Protoss Argus Jewel"));
		}
	}


	return validUpgrades;
}

std::vector<BWAPI::UnitType> StarcraftRules::getValidUnits(const State& s)
{
	std::vector<BWAPI::UnitType> validUnits;

	// Loop through all buildings that have been build in order to find out which units can be build
	for (int i = 0; i < s.getBuildingSequence().size(); i++)
	{
		BWAPI::UnitType ut = s.getBuildingSequence().at(i);

		if (ut.getName() == "Protoss Gateway")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Zealot);
		}
		else if (ut.getName() == "Protoss Cybernetics Core")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Dragoon);
		}
		else if (ut.getName() == "Protoss Templar Archives")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Dark_Templar);
			validUnits.push_back(BWAPI::UnitTypes::Protoss_High_Templar);
		}
		else if (ut.getName() == "Protoss Robotics Facility")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Shuttle);
		}
		else if (ut.getName() == "Protoss Robotics Support Bay")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Reaver);
		}
		else if (ut.getName() == "Protoss Observatory")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Observer);
		}
		else if (ut.getName() == "Protoss Stargate")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Scout);
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Corsair);
		}
		else if (ut.getName() == "Protoss Arbiter Tribunal")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Arbiter);
		}
		else if (ut.getName() == "Protoss Fleet Beacon")
		{
			validUnits.push_back(BWAPI::UnitTypes::Protoss_Carrier);
		}
	}

	return validUnits;
}

std::vector<BWAPI::UnitType> StarcraftRules::getValidBuildings(const State& s)
{
	std::vector<BWAPI::UnitType> validBuildings;
	
	// These buildings are availabel at all time, so we add them to the list of valid buildings to build
	
	BWAPI::UnitType nexus = BWAPI::UnitTypes::Protoss_Nexus;
	BWAPI::UnitType gateway = BWAPI::UnitTypes::Protoss_Gateway;
	
	validBuildings.push_back(nexus);
	validBuildings.push_back(gateway);

	bool assimilatorFound = false;
	bool forgeFound = false;
	bool cyberFound = false;
	bool suportFound = false;
	bool observatoryFound = false;
	bool citadelFound = false;

	for (int i = 0; i < s.getBuildingSequence().size(); i++)
	{
		BWAPI::UnitType ut = s.getBuildingSequence().at(i);

		if (ut.getName() == "Protoss Assimilator")
		{
			assimilatorFound = true;
		}
		else if (ut.getName() == "Protoss Forge")
		{
			forgeFound = true;
		}
		else if (ut.getName() == "Protoss Cybernetics Core")
		{
			cyberFound = true;
		}
		else if (ut.getName() == "Protoss Observatory")
		{
			observatoryFound = true;
		}
		else if (ut.getName() == "Protoss Robotics Support Bay")
		{
			suportFound = true;
		}
		else if (ut.getName() == "Protoss Citadel of Adun")
			citadelFound = true;
	}
	// We loop through each building that has been build. For each building, we add the buildings that, that building unlocks to the list of valid buildings.
	for (int i = 0; i < s.getBuildingSequence().size(); i++)
	{
		BWAPI::UnitType ut = s.getBuildingSequence().at(i);

		if (ut.getName() == "Protoss Forge")
		{
			validBuildings.push_back(BWAPI::UnitTypes::Protoss_Photon_Cannon);
		}
		else if (ut.getName() == "Protoss Gateway")
		{
			if (cyberFound == false)
				validBuildings.push_back(BWAPI::UnitTypes::Protoss_Cybernetics_Core);
		}
		else if (ut.getName() == "Protoss Cybernetics Core"  && assimilatorFound == true)
		{
			validBuildings.push_back(BWAPI::UnitTypes::Protoss_Stargate);
			if (citadelFound == false)
				validBuildings.push_back(BWAPI::UnitTypes::Protoss_Citadel_of_Adun);
			validBuildings.push_back(BWAPI::UnitTypes::Protoss_Robotics_Facility);
		}
		else if (ut.getName() == "Protoss Robotics Facility" && assimilatorFound == true)
		{
			if (suportFound == false)
				validBuildings.push_back(BWAPI::UnitTypes::Protoss_Robotics_Support_Bay);
			if (observatoryFound == false)
				validBuildings.push_back(BWAPI::UnitTypes::Protoss_Observatory);
		}
		else if (ut.getName() == "Protoss Stargate"  && assimilatorFound == true)
		{
			validBuildings.push_back(BWAPI::UnitTypes::Protoss_Fleet_Beacon);
		}
		else if (ut.getName() == "Protoss Citadel Of Adun"  && assimilatorFound == true)
		{
			validBuildings.push_back(BWAPI::UnitTypes::Protoss_Templar_Archives);
		}
		else if (ut.getName() == "Protoss Templar Archives"  && assimilatorFound == true)
		{
			validBuildings.push_back(BWAPI::UnitTypes::Protoss_Arbiter_Tribunal);
		}
	}

	if (assimilatorFound == false)
	{
		BWAPI::UnitType assimilator = BWAPI::UnitTypes::Protoss_Assimilator;
		validBuildings.push_back(assimilator);
	}
	if (forgeFound == false)
	{
		BWAPI::UnitType forge = BWAPI::UnitTypes::Protoss_Forge;
		validBuildings.push_back(forge);
	}


	return validBuildings;
}