#include "CombatGene.h"


CombatGene::CombatGene(void)
{
}

CombatGene::CombatGene(UnitType type, int count) : unitType(type), amount(count)
{
}

CombatGene::~CombatGene(void)
{
}

UnitType CombatGene::getUnitType(void)
{
	return unitType;
}

int CombatGene::getAmount(void)
{
	return amount;
}