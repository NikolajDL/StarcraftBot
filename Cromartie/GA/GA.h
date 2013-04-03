#pragma once
#include <vector>
#include "Chromosome.h"
#include "State.h"
#include "../Interface.h"
#include "../IEventData.h"
#include "../StateExecutor.h"

class GA
{
private:
	Chromosome currentChromosome;
	State currentState;
	int currentChromosomeIndex;
	int currentStateIndex;
	int prev_score;
	int stateChanges;
	int prev_opponentScore;
	StateExecutor stateExecutor;
	std::vector<Chromosome> population;
	double fitness(State state, int score, int opponentScore);
	void loadGAStatus(void);
	void saveGAStatus(void);
	void makeGAStatusFile(void);
	void generateInitialPopulation(int size);
	void createNextGeneration(void);
	void changeState(void);
public:
	GA(void);
	~GA(void);
	State getCurrentState(void);
	void onGameEnd(bool winner, int score, int scoreOpponent, int elapsedTime, int maxElapsedTime);
	void onStarcraftStart(void);
	void loadPopulation(void);
	void savePopulation(void);
	void onUnitCompleteEvent(IEventDataPtr e);
	void onMorph(IEventDataPtr e);
};

