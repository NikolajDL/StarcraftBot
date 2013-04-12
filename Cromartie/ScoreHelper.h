#pragma once
class ScoreHelper
{
public:
	ScoreHelper(void);
	~ScoreHelper(void);
	static int getPlayerScore(void);
	static int getOpponentScore(void);
	static int getUnitScore(void);
	static int getKillScore(void);
	static int getBuildingScore(void);
};

