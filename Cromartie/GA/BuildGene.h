#pragma once
#include "gene.h"
#include "../Interface.h"

class BuildGene :
	public Gene
{
private:
	BWAPI::UnitType buildingType;
public:
	BuildGene(void);
	BuildGene(BWAPI::UnitType building);
	~BuildGene(void);
	BWAPI::UnitType getBuildingType(void);
};

