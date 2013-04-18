#include "ResearchGene.h"


ResearchGene::ResearchGene(void)
{
}

ResearchGene::ResearchGene(std::tr1::shared_ptr<BWAPI::Type> upgrade) : upgradeType(upgrade)
{
}

ResearchGene::~ResearchGene(void)
{
}

std::tr1::shared_ptr<const BWAPI::Type> ResearchGene::getUpgradeType(void)
{
	return this->upgradeType;
}