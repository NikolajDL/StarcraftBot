#pragma once
#include "gene.h"
#include "UnitType.h"

class BuildGene :
	public Gene
{
private:
	UnitType buildingType;
public:
	BuildGene(void);
	BuildGene(UnitType building);
	~BuildGene(void);
	UnitType getBuildingType(void);
};

