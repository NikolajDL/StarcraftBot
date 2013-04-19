#include "ResearchGene.h"


ResearchGene::ResearchGene(void)
{
}

ResearchGene::ResearchGene(BWAPI::UpgradeType research) : m_upgradeType(research), m_techType(NULL)
{
}

ResearchGene::ResearchGene(BWAPI::TechType research) : m_techType(research), m_upgradeType(NULL)
{
}

ResearchGene::~ResearchGene(void)
{
}

BWAPI::UpgradeType ResearchGene::getUpgradeType(void) const
{
	return this->m_upgradeType;
}

BWAPI::TechType ResearchGene::getTechType(void) const
{
	return this->m_techType;
}