#include "DatabaseManager.h"
#include "BuildGene.h"
#include "ResearchGene.h"
#include "CombatGene.h"
#include "AttackGene.h"
#include <sstream>
#include <memory>
#include <typeinfo>
#include <iostream>
#include "sqlite\sqlite3.h"
#include "boost/lexical_cast.hpp"

#define SQLITE_FILENAME "sqlite.db"

DatabaseManager::DatabaseManager(void)
{
}


DatabaseManager::~DatabaseManager(void)
{
}

void DatabaseManager::insertChromosomes(std::vector<Chromosome> c)
{
	for(size_t i=0;i<c.size();i++)	{
		this->insertChromosome(c.at(i));
	}
}

void DatabaseManager::insertAndReplaceChromosomes(std::vector<Chromosome> c)
{
	this->eraseDatabaseContent();
	this->insertChromosomes(c);
}

Chromosome DatabaseManager::getCurrentChromosome(void)
{
	return selectChromosome(getCurrentChromosomeID());
}

int DatabaseManager::getCurrentChromosomeID(void)
{
	sqlite3_open(SQLITE_FILENAME, &db);
	std::stringstream ss;
	ss.str("");
	ss << "SELECT id FROM chromosomes WHERE fitness=-999;";

	sqlite3_stmt* id_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&id_stmt,
		0);
	
	int id = 0;
	if(sqlite3_step(id_stmt) == SQLITE_ROW)
	{
		id = sqlite3_column_int(id_stmt, 0);
	}
	sqlite3_finalize(id_stmt);
	return id;
}

Chromosome DatabaseManager::selectChromosome(int id)
{
	sqlite3_open(SQLITE_FILENAME, &db);
	std::vector<Chromosome> result;
	std::stringstream ss;
	ss.str("");
	ss << "SELECT id, fitness FROM chromosomes WHERE id=" << id << ";";

	sqlite3_stmt* chromosome_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&chromosome_stmt,
		0);

	Chromosome c;
	while(sqlite3_step(chromosome_stmt) == SQLITE_ROW)
	{
		std::vector<BWAPI::UnitType> buildSequence;
		int chromosomeID = sqlite3_column_int(chromosome_stmt, 0);
		c.setId(chromosomeID);
		double chromosomeFitness = sqlite3_column_double(chromosome_stmt, 1);
		c.setFitness(chromosomeFitness);

		ss.str("");
		ss << "SELECT id, fitness FROM states WHERE chromosomes_id = " << chromosomeID << ";";

		sqlite3_stmt* state_stmt;
		sqlite3_prepare_v2(db,
			ss.str().c_str(),
			-1,
			&state_stmt,
			0);

		while(sqlite3_step(state_stmt) == SQLITE_ROW)
		{
			State s(buildSequence);
			int stateID = sqlite3_column_int(state_stmt, 0);
			s.setId(stateID);
			s.setFitness(sqlite3_column_double(state_stmt, 1));

			ss.str("");
			ss << "SELECT id FROM genes WHERE states_id = " << stateID << ";";

			sqlite3_stmt* gene_stmt;
			sqlite3_prepare_v2(db,
				ss.str().c_str(),
				-1,
				&gene_stmt,
				0);

			while(sqlite3_step(gene_stmt) == SQLITE_ROW)
			{
				int geneID = sqlite3_column_int(gene_stmt, 0);

				// Determine which type of gene we are dealing with
				ss.str("");
				ss << "SELECT do_attack FROM attack_genes WHERE id = " << geneID << ";";
				sqlite3_stmt* attackgene_stmt;
				sqlite3_prepare_v2(db,
					ss.str().c_str(),
					-1,
					&attackgene_stmt,
					0);

				ss.str("");
				ss << "SELECT unit_type, unit_amount FROM combat_genes WHERE id = " << geneID << ";";
				sqlite3_stmt* combatgene_stmt;
				sqlite3_prepare_v2(db,
					ss.str().c_str(),
					-1,
					&combatgene_stmt,
					0);

				ss.str("");
				ss << "SELECT research_type FROM research_genes WHERE id = " << geneID << ";";
				sqlite3_stmt* researchgene_stmt;
				sqlite3_prepare_v2(db,
					ss.str().c_str(),
					-1,
					&researchgene_stmt,
					0);

				ss.str("");
				ss << "SELECT building_type FROM build_genes WHERE id = " << geneID << ";";
				sqlite3_stmt* buildgene_stmt;
				sqlite3_prepare_v2(db,
					ss.str().c_str(),
					-1,
					&buildgene_stmt,
					0);

				if(sqlite3_step(attackgene_stmt) == SQLITE_ROW)
				{
					bool doAttack = sqlite3_column_int(attackgene_stmt, 0);
					std::tr1::shared_ptr<AttackGene> g(new AttackGene(doAttack));
					g->setId(geneID);
					s.addGene(g);
				} else if(sqlite3_step(combatgene_stmt) == SQLITE_ROW)
				{
					std::string unittype = boost::lexical_cast<std::string>(sqlite3_column_text(combatgene_stmt,0)); 
					int unitamount = sqlite3_column_int(combatgene_stmt, 1);
					std::tr1::shared_ptr<CombatGene> g(new CombatGene(BWAPI::UnitTypes::getUnitType(unittype), unitamount));
					g->setId(geneID);
					s.addGene(g);
				} else if(sqlite3_step(researchgene_stmt) == SQLITE_ROW)
				{
					std::string upgradetype = boost::lexical_cast<std::string>(sqlite3_column_text(researchgene_stmt,0));
					if(BWAPI::UpgradeTypes::getUpgradeType(upgradetype) != BWAPI::UpgradeTypes::Unknown)
					{
						std::tr1::shared_ptr<ResearchGene> g(new ResearchGene(BWAPI::UpgradeTypes::getUpgradeType(upgradetype)));
						g->setId(geneID);
						s.addGene(g);
					}
					else if(BWAPI::TechTypes::getTechType(upgradetype) != BWAPI::TechTypes::Unknown)
					{
						std::tr1::shared_ptr<ResearchGene> g(new ResearchGene(BWAPI::TechTypes::getTechType(upgradetype)));
						g->setId(geneID);
						s.addGene(g);
					}
					else
					{
						//BWAPI::Broodwar->sendText("DatabaseManager::selectAllChromosomes(): Unable to determine research type");
					}
					
				} else if(sqlite3_step(buildgene_stmt) == SQLITE_ROW)
				{
					std::string buildingtype = boost::lexical_cast<std::string>(sqlite3_column_text(buildgene_stmt,0));
					std::tr1::shared_ptr<BuildGene> g(new BuildGene(BWAPI::UnitTypes::getUnitType(buildingtype)));
					g->setId(geneID);
					s.addGene(g);
				} else
				{
					//BWAPI::Broodwar->sendText("DatabaseManager::selectAllChromosomes(): Found unknown gene type in DB with ID = " + geneID);
				}

				sqlite3_finalize(attackgene_stmt);
				sqlite3_finalize(combatgene_stmt);
				sqlite3_finalize(researchgene_stmt);
				sqlite3_finalize(buildgene_stmt);

			}
			c.addState(s);
			buildSequence = s.getBuildingSequence();
			sqlite3_finalize(gene_stmt);
		}
		sqlite3_finalize(state_stmt);
		result.push_back(c);
	}	
	
	sqlite3_finalize(chromosome_stmt);
	sqlite3_close(db);

	return c;
}

void DatabaseManager::eraseDatabaseContent(void)
{
	sqlite3_open(SQLITE_FILENAME, &db);

	std::stringstream ss;

	ss.str("");
	ss << "BEGIN;";
	sqlite3_stmt* begin_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&begin_stmt,
		0);
	sqlite3_step(begin_stmt);
	sqlite3_finalize(begin_stmt);

	ss.str("");
	ss << "DELETE FROM attack_genes;";
	sqlite3_stmt* delete_attackgenes_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&delete_attackgenes_stmt,
		0);
	sqlite3_step(delete_attackgenes_stmt);
	sqlite3_finalize(delete_attackgenes_stmt);

	ss.str("");
	ss << "DELETE FROM combat_genes;";
	sqlite3_stmt* delete_combatgenes_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&delete_combatgenes_stmt,
		0);
	sqlite3_step(delete_combatgenes_stmt);
	sqlite3_finalize(delete_combatgenes_stmt);

	ss.str("");
	ss << "DELETE FROM build_genes;";
	sqlite3_stmt* delete_buildgenes_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&delete_buildgenes_stmt,
		0);
	sqlite3_step(delete_buildgenes_stmt);
	sqlite3_finalize(delete_buildgenes_stmt);
	
	ss.str("");
	ss << "DELETE FROM research_genes;";
	sqlite3_stmt* delete_researchgenes_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&delete_researchgenes_stmt,
		0);
	sqlite3_step(delete_researchgenes_stmt);
	sqlite3_finalize(delete_researchgenes_stmt);
	
	ss.str("");
	ss << "DELETE FROM genes;";
	sqlite3_stmt* delete_genes_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&delete_genes_stmt,
		0);
	sqlite3_step(delete_genes_stmt);
	sqlite3_finalize(delete_genes_stmt);

	ss.str("");
	ss << "DELETE FROM states;";
	sqlite3_stmt* delete_states_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&delete_states_stmt,
		0);
	sqlite3_step(delete_states_stmt);
	sqlite3_finalize(delete_states_stmt);
	
	ss.str("");
	ss << "DELETE FROM chromosomes;";
	sqlite3_stmt* delete_chromosomes_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&delete_chromosomes_stmt,
		0);
	sqlite3_step(delete_chromosomes_stmt);
	sqlite3_finalize(delete_chromosomes_stmt);

	ss.str("");
	ss << "COMMIT;";
	sqlite3_stmt* commit_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&commit_stmt,
		0);
	sqlite3_step(commit_stmt);
	sqlite3_finalize(commit_stmt);

	sqlite3_close(db);
}

void DatabaseManager::insertChromosome(Chromosome c)
{
	sqlite3_open(SQLITE_FILENAME, &db);

	std::stringstream ss;

	ss.str("");
	ss << "BEGIN;";
	sqlite3_stmt* begin_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&begin_stmt,
		0);
	sqlite3_step(begin_stmt);
	sqlite3_finalize(begin_stmt);

	ss.str("");
	ss << "INSERT INTO chromosomes(fitness) VALUES(" << c.getFitness() << ");";
	sqlite3_stmt* chromosome_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&chromosome_stmt,
		0);

	sqlite3_step(chromosome_stmt);
	sqlite3_finalize(chromosome_stmt);

	int chromosomeID = sqlite3_last_insert_rowid(db);
	for(size_t i=0;i<c.getStates().size();i++)
	{
		ss.str("");
		ss << "INSERT INTO states(chromosomes_id, fitness) VALUES(" << chromosomeID << ", " << c.getStates().at(i).getFitness() <<");";
		sqlite3_stmt* state_stmt;
		sqlite3_prepare_v2(db,
			ss.str().c_str(),
			-1,
			&state_stmt,
			0);
		sqlite3_step(state_stmt);
		sqlite3_finalize(state_stmt);
		int stateID = sqlite3_last_insert_rowid(db);

		for(size_t j=0;j<c.getStates().at(i).getGenes().size();j++)
		{
			ss.str("");
			ss << "INSERT INTO genes(states_id) VALUES(" << stateID << ");";
			sqlite3_stmt* gene_stmt;
			sqlite3_prepare_v2(db,
				ss.str().c_str(),
				-1,
				&gene_stmt,
				0);
			sqlite3_step(gene_stmt);
			sqlite3_finalize(gene_stmt);
			int geneID = sqlite3_last_insert_rowid(db);

			std::tr1::shared_ptr<Gene> g = c.getStates().at(i).getGenes().at(j);
			ss.str("");
			if(typeid(*g) == typeid(BuildGene))
			{
				ss	<< "INSERT INTO build_genes(id, building_type) VALUES(" 
					<< geneID << ","
					<< "\"" << dynamic_cast<BuildGene&>(*g).getBuildingType().getName() << "\""
					<< ");";
			} 
			else if(typeid(*g) == typeid(AttackGene))
			{
				ss	<< "INSERT INTO attack_genes(id, do_attack) VALUES(" 
					<< geneID << ","
					<< dynamic_cast<AttackGene&>(*g).getAttack() 
					<< ");";
			}
			else if(typeid(*g) == typeid(CombatGene))
			{
				ss	<< "INSERT INTO combat_genes(id, unit_type, unit_amount) VALUES(" 
					<< geneID << ","
					<< "\"" << dynamic_cast<CombatGene&>(*g).getUnitType().getName() << "\"" << ", " 
					<< dynamic_cast<CombatGene&>(*g).getAmount() 
					<< ");";
			}
			else if(typeid(*g) == typeid(ResearchGene))
			{
				BWAPI::UpgradeType ut = dynamic_cast<ResearchGene&>(*g).getUpgradeType();
				BWAPI::TechType tt = dynamic_cast<ResearchGene&>(*g).getTechType();

				if(ut != BWAPI::UpgradeTypes::None && tt == BWAPI::TechTypes::None)
				{
					ss	<< "INSERT INTO research_genes(id, research_type) VALUES(" 
					<< geneID << ","
					<< "\"" << ut.getName() << "\""
					<< ");";
				}
				else if(tt != BWAPI::TechTypes::None && ut == BWAPI::UpgradeTypes::None)
				{
					ss	<< "INSERT INTO research_genes(id, research_type) VALUES(" 
					<< geneID << ","
					<< "\"" << tt.getName() << "\""
					<< ");";
				}
				else
				{
					BWAPI::Broodwar->sendText("DatabaseManager::insertChromosome(): Unknown research or tech type");
				}
			}

			sqlite3_stmt* derivedgene_stmt;
			sqlite3_prepare_v2(db,
				ss.str().c_str(),
				-1,
				&derivedgene_stmt,
				0);
			sqlite3_step(derivedgene_stmt);
			sqlite3_finalize(derivedgene_stmt);
		}
	}

	ss.str("");
	ss << "COMMIT;";
	sqlite3_stmt* commit_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&commit_stmt,
		0);
	sqlite3_step(commit_stmt);
	sqlite3_finalize(commit_stmt);

	sqlite3_close(db);
}

void DatabaseManager::updateChromosome(Chromosome c)
{
	sqlite3_open(SQLITE_FILENAME, &db);

	std::stringstream ss;

	ss.str("");
	ss << "BEGIN;";
	sqlite3_stmt* begin_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&begin_stmt,
		0);
	sqlite3_step(begin_stmt);
	sqlite3_finalize(begin_stmt);

	
	ss.str("");
	ss	<< "UPDATE chromosomes "
		<< "SET fitness=" << c.getFitness() << " "
		<< "WHERE id=" << c.getId() << ";";
	sqlite3_stmt* chromosome_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&chromosome_stmt,
		0);
	sqlite3_step(chromosome_stmt);
	sqlite3_finalize(chromosome_stmt);

	for(size_t j=0; j<c.getStates().size();j++)
	{
		State s = c.getStates().at(j);

		ss.str("");
		ss	<< "UPDATE states "
			<< "SET fitness=" << s.getFitness() << " "
			<< "WHERE id=" << s.getId() << ";";
		sqlite3_stmt* state_stmt;
		sqlite3_prepare_v2(db,
			ss.str().c_str(),
			-1,
			&state_stmt,
			0);
		sqlite3_step(state_stmt);
		sqlite3_finalize(state_stmt);

	}

	ss.str("");
	ss << "COMMIT;";
	sqlite3_stmt* commit_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&commit_stmt,
		0);
	sqlite3_step(commit_stmt);
	sqlite3_finalize(commit_stmt);

	sqlite3_close(db);
}


void DatabaseManager::updateChromosomes(std::vector<Chromosome> c)
{
	for(size_t i=0; i<c.size();i++)
	{
		updateChromosome(c.at(i));
	}
}

// If you wanna avoid having a brain aneurysm, avoid reading this method. 
// Multiple nested while-loops, no reuse of statements, lots of fun times to be had here.
std::vector<Chromosome> DatabaseManager::selectAllChromosomes(void)
{
	sqlite3_open(SQLITE_FILENAME, &db);

	std::vector<Chromosome> result;

	std::stringstream ss;

	ss.str("");
	ss << "SELECT id FROM chromosomes;";

	sqlite3_stmt* chromosome_stmt;
	sqlite3_prepare_v2(db,
		ss.str().c_str(),
		-1,
		&chromosome_stmt,
		0);

	while(sqlite3_step(chromosome_stmt) == SQLITE_ROW)
	{
		result.push_back(selectChromosome(sqlite3_column_int(chromosome_stmt, 0)));
	}	
	
	sqlite3_finalize(chromosome_stmt);
	sqlite3_close(db);
	return result;
}
