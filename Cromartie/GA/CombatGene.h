#pragma once
#include "gene.h"
#include "../Interface.h"

class CombatGene :
	public Gene
{
private:
	int amount;
	BWAPI::UnitType unitType;
public:
	CombatGene(void);
	CombatGene(BWAPI::UnitType type, int count);
	~CombatGene(void);
	BWAPI::UnitType getUnitType(void);
	int getAmount(void);
};

