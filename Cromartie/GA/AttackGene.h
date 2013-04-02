#pragma once
#include "gene.h"
class AttackGene :
	public Gene
{
public:
	AttackGene(bool attack = false);
	~AttackGene(void);
	bool getAttack() const;
private:
	bool doAttack;
};

