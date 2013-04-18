#pragma once
#include "CombatGene.h"
#include "ResearchGene.h"
#include "AttackGene.h"
#include "BuildGene.h"
#include "State.h"
#include "../Interface.h"

class StarcraftRules
{
private:
	static std::vector<BWAPI::UnitType> getValidBuildings(const State& s);
	static std::vector<BWAPI::UnitType> getValidUnits(const State& s);
	static std::vector<BWAPI::UpgradeType> getValidUpgrades(const State& s);
	static std::vector<BWAPI::TechType> getValidTechs(const State& s);
public:
	static std::tr1::shared_ptr<CombatGene> getValidCombatGene(const State& s, bool& found);
	static std::tr1::shared_ptr<ResearchGene> getValidResearchGene(const State& s, bool& found);
	static std::tr1::shared_ptr<AttackGene> getValidAttackGene(const State& s, bool& found);
	static std::tr1::shared_ptr<BuildGene> getValidBuildGene(const State& s);
};
