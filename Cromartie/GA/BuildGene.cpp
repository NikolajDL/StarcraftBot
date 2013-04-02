#include "BuildGene.h"

BuildGene::BuildGene(void)
{
}

BuildGene::BuildGene(BWAPI::UnitType building) : buildingType(building)
{
}

BuildGene::~BuildGene(void)
{
}

BWAPI::UnitType BuildGene::getBuildingType(void)
{
	return this->buildingType;
}
