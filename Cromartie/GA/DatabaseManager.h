#pragma once
#include "Chromosome.h"
#include "sqlite\sqlite3.h"

//****************************
// Build order for a chromosome
//****************************
//SELECT build_genes.building_type AS builds, genesID, statesID, chromosomeID
//FROM	(SELECT genes.id AS genesID, statesID, chromosomeID
//	FROM (SELECT id AS statesID, chromosomes_id AS chromosomeID
//		FROM states
//		WHERE chromosomes_id=7)
//	INNER JOIN genes
//	ON statesID=genes.states_id)
//LEFT JOIN build_genes
//ON build_genes.id=genesID

class DatabaseManager
{
public:
	DatabaseManager(void);
	~DatabaseManager(void);
	void insertChromosomes(std::vector<Chromosome> c);
	void insertAndReplaceChromosomes(std::vector<Chromosome> c);
	void updateChromosome(Chromosome c);
	void eraseDatabaseContent(void);
	std::vector<Chromosome> selectAllChromosomes(void);
	Chromosome getCurrentChromosome(void);
private:
	void insertChromosomeNoOpen(Chromosome c);
	void updateStateNoOpen(State s);
	Chromosome selectChromosome(int id);
	int getCurrentChromosomeID(void);
	sqlite3* db;
};

