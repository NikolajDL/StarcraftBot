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
		unit->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
	{
		changeState();
	}
}

void GAClass::onUnitCompleteEvent(IEventDataPtr e)
{
	std::tr1::shared_ptr<UnitCompleteEvent> pEventData = std::tr1::static_pointer_cast<UnitCompleteEvent>(e);
	BWAPI::Unit* unit = pEventData->m_Unit;

	if (unit->getPlayer() == BWAPI::Broodwar->self() &&
		unit->getType().isBuilding() == true &&
		unit->getType().isResourceContainer() == false &&
		unit->getType() != BWAPI::UnitTypes::Protoss_Pylon)
	{
		changeState();
	}
}

void GAClass::changeState()
{
	if(currentStateIndex < CHROMOSOME_LENGTH)
	{
		State s = mChromosome.getState(currentStateIndex);
		s.setFitness(fitness(ScoreHelper::getPlayerScore(), ScoreHelper::getOpponentScore()));
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

	// Hvis nu vi st�r i det f�rste state, er formlen lidt anderledes
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

	Stats::logInidivdualGame(winner, fitness, elapsedTime, BWAPI::Broodwar->self()->getUnitScore(), BWAPI::Broodwar->self()->getKillScore(), BWAPI::Broodwar->self()->getBuildingScore());

	if(db.getCurrentChromosomeID() == 0)
	{
		createNextGeneration();
	}
	
	
	saveGAStatus();
}

void GAClass::update(IEventDataPtr e)
{
	if (stateChanges < 1 )
	{
		stateChanges++;
		BWAPI::Broodwar->sendText("Skipping state change");
		stateExecutor.executeState(mChromosome.getState(currentStateIndex));
	}
}

void GAClass::onStarcraftStart()
{
	this->loadGAStatus();
	if (this->status == 0) // 0 = FirstRun
	{
		std::vector<Chromosome> pop = this->generateInitialPopulation(POP_SIZE);
		this->db.insertAndReplaceChromosomes(pop);
		this->status = 1; // 1 = running
		this->saveGAStatus();
	}

	this->mChromosome = this->db.getCurrentChromosome();
}


void GAClass::createNextGeneration()
{
	// Check if anybody else is generating a generation
	std::ifstream fileExists(GENERATION_PROGRESS.c_str());
	if (!fileExists) {
		std::ofstream myfile (GENERATION_PROGRESS.c_str());
		if (myfile.is_open())
		{
			myfile.close();
		}

		// MUTATION STUFF HERE

		while( remove( GENERATION_PROGRESS.c_str() ) != 0 )
		{
			Sleep( 1000 );
		}

	}
	else {
		// Someone else is generating a generation, wait for them to finish
		while(fileExists.good()) 
		{
			fileExists.close();
			Sleep( 1000 );
			fileExists.open(GENERATION_PROGRESS.c_str());
		}
	}
	fileExists.close();
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
	std::ofstream myfile;
	myfile.open(STATUS_FILE, std::ios::out);
	while(myfile.is_open() == false)
	{
		myfile.open(STATUS_FILE, std::ios::out);
	}

		myfile << "0\n"; // Status: FirstRun
		myfile.close();
}

void GAClass::saveGAStatus()
{
	//std::ostringstream convert;
	std::ofstream myfile;
	myfile.open(STATUS_FILE, std::ios::out);
	while(myfile.is_open() == false)
	{
		myfile.open(STATUS_FILE, std::ios::out);
	}

		myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << status) )->str() << "\n";
		myfile.close();
	
}

void GAClass::loadGAStatus()
{
	// If there is no state.txt file, we make it
	std::ifstream fileExists(STATUS_FILE);
	if (!fileExists) {
		makeGAStatusFile();
	}
	fileExists.close();

	// Parsing the state.txt file
	std::string line;
	std::ifstream myfile (STATUS_FILE);
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
