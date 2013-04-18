#pragma once
#include "gene.h"
#include "../Interface.h"

class ResearchGene :
	public Gene
{
private:
	BWAPI::UpgradeType m_upgradeType;
	BWAPI::TechType m_techType;
public:
	ResearchGene(void);
	ResearchGene(BWAPI::UpgradeType research);
	ResearchGene(BWAPI::TechType research);
	~ResearchGene(void);
	BWAPI::UpgradeType getUpgradeType(void) const;
	BWAPI::TechType getTechType(void) const;
};

