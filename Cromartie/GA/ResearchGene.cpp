#include "ResearchGene.h"


ResearchGene::ResearchGene(void)
{
}

ResearchGene::ResearchGene(BWAPI::UpgradeType upgrade) : upgradeType(upgrade)
{
}

ResearchGene::~ResearchGene(void)
{
}

BWAPI::UpgradeType ResearchGene::getUpgradeType(void) const
{
	return this->upgradeType;
}