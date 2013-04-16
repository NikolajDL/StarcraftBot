//#include "FlatFileSerializer.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include "GA/AttackGene.h"
//#include "GA/BuildGene.h"
//#include "GA/CombatGene.h"
//#include "GA/ResearchGene.h"
//
//FlatFileSerializer::FlatFileSerializer(void)
//{
//}
//
//
//FlatFileSerializer::~FlatFileSerializer(void)
//{
//}
//
//void FlatFileSerializer::savePop(std::vector<Chromosome> pop)
//{
//	std::ostringstream convert;
//
//	std::ofstream myfile ("pop.txt");
//
//	myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << pop.size()) )->str() << "\n";
//
//	for (int i = 0; i < pop.size(); i++)
//	{
//		myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << pop.at(i).getFitness()) )->str() << "\n";
//		myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << pop.at(i).getStates().size()) )->str() << "\n";
//		
//		for (int j = 0; j < pop.at(i).getStates().size(); j++)
//		{
//			myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << pop.at(i).getStates().at(j).getGenes().size()) )->str() << "\n";
//			
//			//myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << pop.at(i).getStates().at(j).getBuildingSequence().size()) )->str() << "\n";
//			//for (int l = 0; l < pop.at(i).getStates().at(j).getBuildingSequence().size(); l++)
//			//{
//			//	
//			//}
//
//			for (int k = 0; k < pop.at(i).getStates().at(j).getGenes().size(); k++)
//			{
//				std::tr1::shared_ptr<Gene> g = pop.at(i).getStates().at(j).getGenes().at(k);
//
//				if(typeid(*g) == typeid(AttackGene))
//				{
//					AttackGene gene = dynamic_cast<AttackGene&>(*g);
//					myfile << "attack" << "\n";
//				}
//				else if (typeid(*g) == typeid(CombatGene))
//				{
//					CombatGene gene = dynamic_cast<CombatGene&>(*g);
//					myfile << "combat" << "\n";
//					myfile << gene.getUnitType().getName() << "\n";
//					myfile << static_cast<std::ostringstream*>( &(std::ostringstream() << gene.getAmount()))->str() << "\n";
//				}
//				else if (typeid(*g) == typeid(ResearchGene))
//				{
//					ResearchGene gene = dynamic_cast<ResearchGene&>(*g);
//					myfile << "research" << "\n";
//					myfile << gene.getUpgradeType().getName() << "\n";
//				}
//				else if (typeid(*g) == typeid(BuildGene))
//				{
//					BuildGene gene = dynamic_cast<BuildGene&>(*g);
//					myfile << "build" << "\n";
//					myfile << gene.getBuildingType().getName()<< "\n";;
//				}
//			}
//		}
//		
//	}
//
//	myfile.close();
//}
//
//std::vector<Chromosome> FlatFileSerializer::loadPop()
//{
//	vector<Chromosome> pop;
//
//	std::string line;
//	std::ifstream myfile ("pop.txt");
//
//	std::getline (myfile,line);
//	int chromCount = atoi(line.c_str());
//
//	for (int i = 0; i < chromCount; i++)
//	{
//		Chromosome c;
//
//		std::getline (myfile,line);
//		double fitness = atoi(line.c_str());
//		c.setFitness(fitness);
//
//		std::getline (myfile,line);
//		int statesCount = atoi(line.c_str());
//
//		State prevState;
//		for (int j = 0; j < statesCount; j++)
//		{
//			State state(prevState.getBuildingSequence());
//
//			std::getline (myfile,line);
//			int geneCount = atoi(line.c_str());
//			
//			for (int k = 0; k < geneCount; k++)
//			{
//				std::getline (myfile,line);
//
//				if (line == "attack")
//				{
//					state.addGene(std::tr1::shared_ptr<AttackGene>(new AttackGene(true)));
//				}
//				else if (line == "combat")
//				{
//					std::getline (myfile,line);
//					std::string name = line;
//
//					std::getline (myfile,line);
//					int amount = atoi(line.c_str());
//
//					state.addGene(std::tr1::shared_ptr<CombatGene>(new CombatGene(BWAPI::UnitTypes::getUnitType(name), amount)));
//				}
//				else if (line == "research")
//				{
//					std::getline (myfile,line);
//					ResearchGene gene(BWAPI::UpgradeTypes::getUpgradeType(line));
//
//					state.addGene(std::tr1::shared_ptr<ResearchGene>(new ResearchGene(BWAPI::UpgradeTypes::getUpgradeType(line))));
//				}
//				else if (line == "build")
//				{
//					std::getline (myfile,line);
//
//					state.addGene(std::tr1::shared_ptr<BuildGene>(new BuildGene(BWAPI::UnitTypes::getUnitType(line))));
//					state.getBuildingSequence().push_back(BWAPI::UnitTypes::getUnitType(line));
//				}
//			}
//
//			c.addState(state);
//			prevState = state;
//		}
//
//		pop.push_back(c);
//	}
//
//
//	myfile.close();
//	return pop;
//}