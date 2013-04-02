#include "ResearchGene.h"


ResearchGene::ResearchGene(void)
{
}

ResearchGene::ResearchGene(UpgradeType upgrade) : upgradeType(upgrade)
{
}

ResearchGene::~ResearchGene(void)
{
}

UpgradeType ResearchGene::getUpgradeType(void) const
{
	return this->upgradeType;
}