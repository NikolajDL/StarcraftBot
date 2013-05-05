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
		getCurrentState().setFitness(fitness(ScoreHelper::getPlayerScore(), ScoreHelper::getOpponentScore()));
		currentStateIndex++;
		stateChanges++;

		if(!stateExecutor.executeState(getCurrentState()))
			changeState();
	}
	else if(currentStateIndex >= CHROMOSOME_LENGTH)
	{
		//EQUEUE( new ChangeBuildOrderEvent(BuildOrderID::PvPEndGame));
		//EQUEUE( new ToggleOrderEvent(Order::MacroArmyProduction) );
		//EQUEUE( new ContinueBuildOrderEvent() );
	}	
}

Chromosome& GAClass::getCurrentChromosome()
{
	return population.at(currentChromosomeIndex);
}

State& GAClass::getCurrentState()
{
	return getCurrentChromosome().getState(currentStateIndex);
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
	// Avoid starting up new threads, before the current worker thread has completed
	while(!threadFinished){ Sleep(1000); }

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

	

	bool nonTestedChromosomeFound = false;
	for (size_t i = 0; i < population.size(); i++)
	{
		if (population.at(i).getFitness() == -999)
		{
			nonTestedChromosomeFound = true;
			break;
		}
	}
	if (nonTestedChromosomeFound == false)
	{
		status = 2; // 2 = finishedGeneration
	}

	getCurrentChromosome().setFitness(fitness);
	savePopulation();
	saveGAStatus();
	Stats::logPop(population, elapsedTime, winner);
}

static DWORD WINAPI GAThread(LPVOID lpParam)
{
	GAClass* This = (GAClass*)lpParam;
	if(!This->threadFinished)
	{
		This->loadGAStatus();
		if (This->status == 0) // 0 = FirstRun
		{
			This->generateInitialPopulation(POP_SIZE);
			//status = 1; // 1 = running
		}
		else if (This->status == 1) // 1 = running
		{
			This->population = This->db.selectAllChromosomes();
		}
		else if (This->status == 2) // 2 = finishedGeneration
		{
			This->population = This->db.selectAllChromosomes();
			This->createNextGeneration();
			This->status = 1;
		}

		for (size_t i = 0; i < This->population.size(); i++)
		{
			if (This->population.at(i).getFitness() == -999)
			{
				This->currentChromosomeIndex = i;
				break;
			}
		}
	}

	This->threadFinished = true;
	return 0;
};

void GAClass::update(IEventDataPtr e)
{
	if (stateChanges < 1 && threadFinished)
	{
		stateChanges++;
		BWAPI::Broodwar->sendText("Skipping state change");
		stateExecutor.executeState(getCurrentState());
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

void GAClass::savePopulation()
{
	if(status == 0) 
	{ 
		db.insertChromosomes(population); 
		status = 1; 
	}
	else 
	{ 
		db.updateChromosomes(population); 
	}
}

void GAClass::createNextGeneration()
{
	// Replace this class if you want another selection aglorithm
	TournamentSelection ts;
	ts.selectAndMutate(population);
	db.insertAndReplaceChromosomes(population);
}

void GAClass::generateInitialPopulation(int size)
{
	for (int i = 0; i < size; i++)
	{
		population.push_back(GeneticOperator::RandomChromosome());
	}
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