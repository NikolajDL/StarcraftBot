#pragma once
#include <vector>
#include "Chromosome.h"
#include "State.h"
#include "../Interface.h"
#include "../IEventData.h"
#include "../StateExecutor.h"
#include "DatabaseManager.h"
#include "../Singleton.h"

class GAClass
{
private:

	int currentStateIndex;
	int prev_score;
	int stateChanges;
	int prev_opponentScore;
	StateExecutor stateExecutor;
	
	double fitness(int score, int opponentScore);
	void saveGAStatus(void);
	void makeGAStatusFile(void);
	void changeState(void);
	Chromosome& getCurrentChromosome(void);
	State& getCurrentState(void);
	void savePopulation(void);
	
public:	
	int status; // 0 = FirstRun 1 = Running 2 = FinishedGeneration 3 = Finished
	bool threadFinished;
	std::vector<Chromosome> population;
	int currentChromosomeIndex;
	DatabaseManager db;
	void createNextGeneration(void);
	void loadGAStatus(void);
	void generateInitialPopulation(int size);

	void update(IEventDataPtr e);
	void onGameEnd(bool winner, int score, int scoreOpponent, int elapsedTime, int maxElapsedTime);
	void onStarcraftStart(IEventDataPtr e);
	void onUnitCompleteEvent(IEventDataPtr e);
	void onMorph(IEventDataPtr e);
};

typedef Singleton<GAClass> GA;