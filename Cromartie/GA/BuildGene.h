#pragma once
#include "gene.h"
#include "../Interface.h"
#include <string>

class BuildGene :
	public Gene
{
private:
	BWAPI::UnitType buildingType;
public:
	std::string name;
	BuildGene(void);
	BuildGene(BWAPI::UnitType building);
	~BuildGene(void);
	BWAPI::UnitType getBuildingType(void);
};

