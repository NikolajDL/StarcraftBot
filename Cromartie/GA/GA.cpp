#include "GA.h"
#include "GeneticOperator.h"
#include "TournamentSelection.h"
#include "../GenericEvents.h"
#include "../ScoreHelper.h"
#include "../Stats.h"
#include "../EventManager.h"
#include "../HypothalamusEvents.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Settings.h"


#include <windows.h>

void GAClass::onMorph(IEventDataPtr e)
{
	std::tr1::shared_ptr<UnitMorphEvent> pEventData = std::tr1::static_pointer_cast<UnitMorphEvent>(e);
	BWAPI::Unit* unit = pEventData->m_Unit;

	if (unit->getPlayer() == BWAPI::Broodwar->self() &&
		unit->getType() == BWAPI::UnitTypes::Protoss_Assimilator &&
		threadFinished)
	{
		changeState();
	}
}

int pylonCtr = 0;
void GAClass::onUnitCompleteEvent(IEventDataPtr e)
{
	std::tr1::shared_ptr<UnitCompleteEvent> pEventData = std::tr1::static_pointer_cast<UnitCompleteEvent>(e);
	BWAPI::Unit* unit = pEventData->m_Unit;

	if (unit->getPlayer() == BWAPI::Broodwar->self() &&
		unit->getType().isBuilding() == true &&
		unit->getType().isResourceContainer() == false &&
		unit->getType() != BWAPI::UnitTypes::Protoss_Pylon &&
		threadFinished)
	{
		changeState();
	}
	
	if (unit->getPlayer() == BWAPI::Broodwar->self() &&
		unit->getType() == BWAPI::UnitTypes::Protoss_Pylon)
	{
		pylonCtr++;
		if(pylonCtr > 1 && !threadFinished)
			while(!threadFinished){ Sleep(1000); }
	}
}

void GAClass::changeState()
{
	if(currentStateIndex < CHROMOSOME_LENGTH)
	{
		State s = mChromosome.getState(currentStateIndex);
		s.setFitness(fitness(ScoreHelper::getPlayerScore(), ScoreHelper::getOpponentScore()));
		db.updateState(s);
		currentStateIndex++;
		stateChanges++;

		if(!stateExecutor.executeState(mChromosome.getState(currentStateIndex)))
			changeState();
	}
	else if(currentStateIndex >= CHROMOSOME_LENGTH)
	{
		//EQUEUE( new ChangeBuildOrderEvent(BuildOrderID::PvPEndGame));
		//EQUEUE( new ToggleOrderEvent(Order::MacroArmyProduction) );
		//EQUEUE( new ContinueBuildOrderEvent() );
	}	
}

double GAClass::fitness(int score, int opponentScore)
{
	double fitness;

	// Hvis nu vi står i det første state, er formlen lidt anderledes
	if (currentStateIndex == 0)
	{
		fitness = (double)score / ((double)score + (double)opponentScore);
	}
	else
	{
		fitness = ((double)score / ((double)score + (double)opponentScore)) -
			((double)prev_score / ((double)prev_score + (double)prev_opponentScore));
	}

	prev_score = score;
	prev_opponentScore = opponentScore;

	return fitness;
}

void GAClass::onGameEnd(bool winner, int score, int scoreOpponent, int elapsedTime, int maxElapsedTime)
{
	double fitness = 0;
	if (winner)
	{
		fitness = (double)score / ((double)scoreOpponent);
	}
	else
	{
		fitness = ((double)elapsedTime / (double)maxElapsedTime) * ((double)score / ((double)scoreOpponent));
	}

	fitness *= 10000;
	mChromosome.setFitness(fitness);
	db.updateChromosome(mChromosome);

	if(db.getCurrentChromosome().getId() == 0)
	{
		status = 2; // 2 = finishedGeneration
	}
	
	saveGAStatus();
}

static DWORD WINAPI GAThread(LPVOID lpParam)
{
	GAClass* This = (GAClass*)lpParam;

	This->loadGAStatus();
	if (This->status == 0) // 0 = FirstRun
	{
		std::vector<Chromosome> pop = This->generateInitialPopulation(POP_SIZE);
		This->db.insertAndReplaceChromosomes(pop);
		This->status = 1; // 1 = running
	}
	else if (This->status == 1) // 1 = running
	{
		//This->population = This->db.selectAllChromosomes();
	}
	else if (This->status == 2) // 2 = finishedGeneration
	{
		This->createNextGeneration();
		This->status = 1;
	}

	This->mChromosome = This->db.getCurrentChromosome();
	This->threadFinished = true;
	return 0;
};

void GAClass::update(IEventDataPtr e)
{
	if (threadFinished && stateChanges < 1 )
	{
		stateChanges++;
		BWAPI::Broodwar->sendText("Skipping state change");
		stateExecutor.executeState(mChromosome.getState(currentStateIndex));
	}
}

void GAClass::onStarcraftStart(IEventDataPtr e)
{
	this->threadFinished = false;
	CreateThread( 
            NULL,           // default security attributes
            0,              // use default stack size  
            GAThread,       // thread function name
            (void*) this,   // argument to thread function 
            0,              // use default creation flags 
            NULL);			// returns the thread identifier 
}


void GAClass::createNextGeneration()
{
	std::vector<Chromosome> pop = db.selectAllChromosomes();
	Stats::logPop(pop, 0, false);

	// Replace this class if you want another selection aglorithm
	TournamentSelection ts;
	ts.selectAndMutate(pop);
	db.insertAndReplaceChromosomes(pop);
}

std::vector<Chromosome> GAClass::generateInitialPopulation(int size)
{
	std::vector<Chromosome> result;
	for (int i = 0; i < size; i++)
	{
		result.push_back(GeneticOperator::RandomChromosome());
	}
	return result;
}

void GAClass::makeGAStatusFile()
{
	std::ofstream myfile ("status.txt");
	if (myfile.is_open())
	{
		myfile << "0\n"; // Status: FirstRun
		myfile.close();
	}
}

void GAClass::saveGAStatus()
{
	std::ostringstream convert;

	std::ofstream myfile ("status.txt");
	if (myfile.is_open())
	{
		myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << status) )->str() << "\n";
		myfile.close();
	}
}

void GAClass::loadGAStatus()
{
	// If there is no state.txt file, we make it
	std::ifstream fileExists("status.txt");
	if (!fileExists) {
		makeGAStatusFile();
	}
	fileExists.close();

	// Parsing the state.txt file
	std::string line;
	std::ifstream myfile ("status.txt");
	if (myfile.is_open())
	{
		// Read the status of the GA
		myfile.good();
		std::getline (myfile,line);
		status = atoi(line.c_str());

		myfile.close();
	}
	//else BWAPI::Broodwar->sendText("Unable to open file"); 

	// Read next chromosome index
	// Read status: FirstRun, Running, FinishedGeneration, Finished

}