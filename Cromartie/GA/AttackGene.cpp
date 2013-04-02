#include "AttackGene.h"


AttackGene::AttackGene(bool attack) : doAttack(attack)
{
}


AttackGene::~AttackGene(void)
{
}

bool AttackGene::getAttack(void) const
{
	return this->doAttack;
}