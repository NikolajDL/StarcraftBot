#pragma once
#include "gene.h"
#include "../Interface.h"

class ResearchGene :
	public Gene
{
private:
	BWAPI::UpgradeType upgradeType;
public:
	ResearchGene(void);
	ResearchGene(BWAPI::UpgradeType upgrade);
	~ResearchGene(void);
	BWAPI::UpgradeType getUpgradeType(void) const;
};

