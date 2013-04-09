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
	//std::set<Player> players = PlayerTracker::Instance().getEnemies();
	//Player enemy;
	//std::set<Player>::iterator it=players.begin();
 //   if (it != players.end())
	//	enemy = *it;

	//int scoreOpponent = enemy->getUnitScore() +
	//	enemy->getKillScore() +
	//	enemy->getBuildingScore();
	int scoreOpponent = 0;

	return scoreOpponent;
}