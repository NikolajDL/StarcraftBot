#include "BuildGene.h"
#include "UnitType.h"

BuildGene::BuildGene(void)
{
}

BuildGene::BuildGene(UnitType building) : buildingType(building)
{
}

BuildGene::~BuildGene(void)
{
}

UnitType BuildGene::getBuildingType(void)
{
	return this->buildingType;
}
