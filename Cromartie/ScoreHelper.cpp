#include "ScoreHelper.h"
#include "Interface.h"
#include "PlayerTracker.h"

ScoreHelper::ScoreHelper(void)
{
}


ScoreHelper::~ScoreHelper(void)
{
}

int ScoreHelper::getPlayerScore(void)
{
	int score = BWAPI::Broodwar->self()->getUnitScore() +
		BWAPI::Broodwar->self()->getKillScore() +
		BWAPI::Broodwar->self()->getBuildingScore();
	return score;
}

int ScoreHelper::getOpponentScore(void)
{
	// TODO: Not sure if this works
	int scoreOpponent = BWAPI::Broodwar->enemy()->getUnitScore() +
		BWAPI::Broodwar->enemy()->getKillScore() +
		BWAPI::Broodwar->enemy()->getBuildingScore();

	return scoreOpponent;
}