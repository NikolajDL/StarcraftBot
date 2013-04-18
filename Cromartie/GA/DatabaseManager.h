#pragma once
#include "Chromosome.h"
#include "sqlite\sqlite3.h"


class DatabaseManager
{
public:
	DatabaseManager(void);
	~DatabaseManager(void);
	void insertChromosome(Chromosome c);
	void insertChromosomes(std::vector<Chromosome> c);
	void insertAndReplaceChromosomes(std::vector<Chromosome> c);
	void updateChromosomes(std::vector<Chromosome> c);
	void eraseDatabaseContent(void);
	std::vector<Chromosome> selectAllChromosomes(void);
private:
	sqlite3* db;
};

