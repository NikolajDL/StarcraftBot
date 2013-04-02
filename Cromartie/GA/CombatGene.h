#pragma once
#include "gene.h"
#include "UnitType.h"

class CombatGene :
	public Gene
{
private:
	int amount;
	UnitType unitType;
public:
	CombatGene(void);
	CombatGene(UnitType type, int count);
	~CombatGene(void);
	UnitType getUnitType(void);
	int getAmount(void);
};

