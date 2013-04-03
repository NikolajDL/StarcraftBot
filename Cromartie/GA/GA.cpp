#include "GA.h"
#include "GeneticOperator.h"
#include "TournamentSelection.h"
#include "DatabaseManager.h"
#include "../GenericEvents.h"
#include "../ScoreHelper.h"
#include <iostream>
#include <fstream>
#include <sstream>

int status; // 0 = FirstRun 1 = Running 2 = FinishedGeneration 3 = Finished

GA::GA(void) : currentStateIndex(0), stateChanges(0), currentState(std::vector<BWAPI::UnitType>())
{
}


GA::~GA(void)
{
}

void GA::onMorph(IEventDataPtr e)
{
	std::tr1::shared_ptr<UnitMorphEvent> pEventData = std::tr1::static_pointer_cast<UnitMorphEvent>(e);
	BWAPI::Unit* unit = pEventData->m_Unit;

	std::string name = unit->getType().getName();

	if (name == "Protoss Assimilator")
	{
		BWAPI::Broodwar->sendText("Changing state");
		BWAPI::Broodwar->sendText(unit->getType().getName().c_str());
		changeState();
	}
}

void GA::onUnitCompleteEvent(IEventDataPtr e)
{
	std::tr1::shared_ptr<UnitCompleteEvent> pEventData = std::tr1::static_pointer_cast<UnitCompleteEvent>(e);
	BWAPI::Unit* unit = pEventData->m_Unit;

	std::string name = unit->getType().getName();

	if (unit->getPlayer() == BWAPI::Broodwar->self() &&
		unit->getType().isBuilding() == true &&
		(unit->getType().isResourceContainer() == false) &&
		name != "Protoss Pylon")
	{
		if (stateChanges < 1)
		{
			stateChanges++;
			BWAPI::Broodwar->sendText("Skipping state change");
			return;
		}

		BWAPI::Broodwar->sendText("Changing state");
		BWAPI::Broodwar->sendText(unit->getType().getName().c_str());
		changeState();
	}
}

void GA::changeState()
{
		
		currentState.setFitness(fitness(currentState, ScoreHelper::getPlayerScore(), ScoreHelper::getOpponentScore()));
		// TODO:
		// GeneExecuter.ExecuteState(state);
}

State GA::getCurrentState()
{
	return currentChromosome.getStates().at(currentStateIndex);
}

double GA::fitness(State state, int score, int opponentScore)
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

void GA::onGameEnd(bool winner, int score, int scoreOpponent, int elapsedTime, int maxElapsedTime)
{
	double fitness = 0;
	if (winner)
	{
		fitness = (double)score / ((double)score + (double)scoreOpponent);
	}
	else
	{
		fitness = ((double)elapsedTime / (double)maxElapsedTime) * ((double)score / ((double)score + (double)scoreOpponent));
	}

	currentChromosome.setFitness(fitness);
	savePopulation();
}

void GA::onStarcraftStart()
{
	loadGAStatus();

	if (status == 0) // 0 = FirstRun
	{
		generateInitialPopulation(50);
		status = 1; // 1 = running
	}
	else if (status == 1) // 1 = running
	{
		loadPopulation();
	}
	else if (status == 2) // 2 = finishedGeneration
	{
		loadPopulation();
		createNextGeneration();
	}

	bool nonTestedChromosomeFound = false;
	for (int i = 0; i < population.size(); i++)
	{
		if (population.at(i).getFitness() == -999)
		{
			currentChromosomeIndex = i;
			currentChromosome = population.at(i);
			nonTestedChromosomeFound = true;
			break;
		}
	}
	if (nonTestedChromosomeFound == false)
	{
		status = 2; // 2 = finishedGeneration
	}
}

void GA::loadPopulation()
{
	DatabaseManager db;

	population = db.selectAllChromosomes();
}

void GA::savePopulation()
{
	DatabaseManager db;

	db.eraseDatabaseContent();

	db.insertChromosomes(population);
}

void GA::createNextGeneration()
{
	// Replace this class if you want another selection aglorithm
	TournamentSelection ts;
	ts.selectAndMutate(population);
}

void GA::generateInitialPopulation(int size)
{
	for (int i = 0; i < size; i++)
	{
		Chromosome c =  GeneticOperator::RandomChromosome();
		population.push_back(c);
	}
}

void GA::makeGAStatusFile()
{
	std::ofstream myfile ("status.txt");
  if (myfile.is_open())
  {
    myfile << "0\n"; // Status: FirstRun
    myfile.close();
  }
  else std::cout << "Unable to open GA state file";
}

void GA::saveGAStatus()
{
	std::ostringstream convert;

	std::ofstream myfile ("status.txt");
  if (myfile.is_open())
  {
	myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << status) )->str() << "\n";
    myfile.close();
  }
  else std::cout << "Unable to open GA state file";
}

void GA::loadGAStatus()
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
  else std::cout << "Unable to open file"; 

	// Read next chromosome index
	// Read status: FirstRun, Running, FinishedGeneration, Finished

}