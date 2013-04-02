#pragma once
#include "CombatGene.h"
#include "ResearchGene.h"
#include "AttackGene.h"
#include "BuildGene.h"
#include "State.h"
#include "UnitType.h"

class StarcraftRules
{
private:
	static std::vector<UnitType> getValidBuildings(const State& s);
	static std::vector<UnitType> getValidUnits(const State& s);
	static std::vector<UpgradeType> getValidUpgrades(const State& s);
public:
	static std::tr1::shared_ptr<CombatGene> getValidCombatGene(const State& s, bool& found);
	static std::tr1::shared_ptr<ResearchGene> getValidResearchGene(const State& s, bool& found);
	static std::tr1::shared_ptr<AttackGene> getValidAttackGene(const State& s, bool& found);
	static std::tr1::shared_ptr<BuildGene> getValidBuildGene(const State& s);
};
