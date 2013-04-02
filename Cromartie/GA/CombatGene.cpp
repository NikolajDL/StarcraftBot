#include "CombatGene.h"


CombatGene::CombatGene(void)
{
}

CombatGene::CombatGene(BWAPI::UnitType type, int count) : unitType(type), amount(count)
{
}

CombatGene::~CombatGene(void)
{
}

BWAPI::UnitType CombatGene::getUnitType(void)
{
	return unitType;
}

int CombatGene::getAmount(void)
{
	return amount;
}