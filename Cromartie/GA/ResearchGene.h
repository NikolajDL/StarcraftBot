#pragma once
#include "gene.h"
#include "UnitType.h"
class ResearchGene :
	public Gene
{
private:
	UpgradeType upgradeType;
public:
	ResearchGene(void);
	ResearchGene(UpgradeType upgrade);
	~ResearchGene(void);
	UpgradeType getUpgradeType(void) const;
};

