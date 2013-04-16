#pragma once
#include "gene.h"
#include "../Interface.h"

class ResearchGene :
	public Gene
{
private:
	std::tr1::shared_ptr<BWAPI::Type> upgradeType;
public:
	ResearchGene(void);
	ResearchGene(std::tr1::shared_ptr<BWAPI::Type> upgrade);
	~ResearchGene(void);
	std::tr1::shared_ptr<BWAPI::Type> getUpgradeType(void) const;
};

