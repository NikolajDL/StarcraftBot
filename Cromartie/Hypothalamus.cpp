#include "Hypothalamus.h"

#include "boost\algorithm\string.hpp"

#include "TaskManager.h"
#include "MacroManager.h"
#include "BuildOrderManager.h"
#include "EventManager.h"

#include "HypothalamusEvents.h"
#include "GenericEvents.h"

void HypothalamusClass::buildUnitEvent(IEventDataPtr evnt)
{
	
	std::tr1::shared_ptr<BuildUnitEvent> pEventData = std::tr1::static_pointer_cast<BuildUnitEvent>(evnt);
	BWAPI::UnitType unit = pEventData->mUnit;
	int number = pEventData->mNumber;
	BuildingLocation location = pEventData->mLocation;
	TaskType taskType = pEventData->mTaskType;

	if(number>=1)
		buildUnit(unit, taskType, number, location);
	else
		buildUnit(unit, taskType, location);
}
void HypothalamusClass::toggleOrderEvent(IEventDataPtr evnt)
{
	std::tr1::shared_ptr<ToggleOrderEvent> pEventData = std::tr1::static_pointer_cast<ToggleOrderEvent>(evnt);
	Order order = pEventData->mOrder;

	toggleOrder(order);
}

void HypothalamusClass::upgradeEvent(IEventDataPtr evnt)
{
	std::tr1::shared_ptr<UpgradeEvent> pEventData = std::tr1::static_pointer_cast<UpgradeEvent>(evnt);
	std::tr1::shared_ptr<const BWAPI::Type> type = pEventData->mType;
	int level = pEventData->mLevel;

	//BWAPI::Broodwar->sendText(typeid(type).name());
	//BWAPI::Broodwar->sendText(typeid(*type).name());
	
	std::tr1::shared_ptr<const BWAPI::UpgradeType> upgradeTypeTmp = std::tr1::static_pointer_cast<const BWAPI::UpgradeType>(type);
	std::tr1::shared_ptr<const BWAPI::TechType> techTypeTmp = std::tr1::static_pointer_cast<const BWAPI::TechType>(type);
	

	BWAPI::Broodwar->sendText(typeid(*type).name());
	BWAPI::Broodwar->sendText(typeid(*upgradeTypeTmp).name());

	if(upgradeTypeTmp!=0){
		BWAPI::Broodwar->sendText(upgradeTypeTmp->getName().c_str());
		upgrade(*upgradeTypeTmp, 1);
	}
	else if(techTypeTmp!=0){
		BWAPI::Broodwar->sendText(techTypeTmp->getName().c_str());
		upgrade(*techTypeTmp);
	}




	if(typeid(*type) == typeid(BWAPI::UpgradeType))
	{
		BWAPI::Broodwar->sendText("WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!!!!!! 1111111111");
		std::tr1::shared_ptr<const BWAPI::UpgradeType> upgradeType = std::tr1::static_pointer_cast<const BWAPI::UpgradeType>(type);
		upgrade(*upgradeType, level);
	}else if(typeid(*type) == typeid(BWAPI::TechType))
	{
		BWAPI::Broodwar->sendText("WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!!!!!! 2222222222");
		std::tr1::shared_ptr<const BWAPI::TechType> techType = std::tr1::static_pointer_cast<const BWAPI::TechType>(type);
		upgrade(*techType);
	}
}
void HypothalamusClass::setArmyBehaviourEvent(IEventDataPtr evnt)
{
	std::tr1::shared_ptr<SetArmyBehaviourEvent> pEventData = std::tr1::static_pointer_cast<SetArmyBehaviourEvent>(evnt);

	setArmyBehaviour(pEventData->mArmyBehaviour);
}
void HypothalamusClass::addProductionEvent(IEventDataPtr evnt)
{
	std::tr1::shared_ptr<AddProductionEvent> pEventData = std::tr1::static_pointer_cast<AddProductionEvent>(evnt);

	addProduction(pEventData->mUnit);
}


void HypothalamusClass::buildUnit(BWAPI::UnitType unit, BuildingLocation position)
{
	TaskManager::Instance().build(unit, TaskType::Highest, position);
}
void HypothalamusClass::buildUnit(BWAPI::UnitType unit, TaskType taskType, BuildingLocation position)
{
	TaskManager::Instance().build(unit, taskType, position);
}
void HypothalamusClass::buildUnit(BWAPI::UnitType unit, int number, BuildingLocation position)
{
	for(int i = 0; i < number; ++i)
		TaskManager::Instance().build(unit, TaskType::Highest, position);
}
void HypothalamusClass::buildUnit(BWAPI::UnitType unit, TaskType taskType, int number, BuildingLocation position)
{
	for(int i = 0; i < number; ++i)
		TaskManager::Instance().build(unit, taskType, position);
}
void HypothalamusClass::upgrade(BWAPI::UpgradeType type, int level)
{
	TaskManager::Instance().upgrade(type, level, TaskType::Highest);
}
void HypothalamusClass::upgrade(BWAPI::TechType type)
{
	TaskManager::Instance().research(type, TaskType::Highest);
}
void HypothalamusClass::setArmyBehaviour(ArmyBehaviour armyBehaviour)
{
	SquadManager::Instance().setBehaviour(armyBehaviour);
}
void HypothalamusClass::toggleOrder(Order order)
{
	BuildOrderManager::Instance().toggleOrder(order);
}
void HypothalamusClass::addProduction(BWAPI::UnitType unit)
{
	mProduces.push_back(UnitToProduce(unit, 1, 100, Condition(ConditionTest::None, true), Condition(ConditionTest::None, true)));
	MacroManager::Instance().setUnitsToProduce(mProduces);
}
void HypothalamusClass::attack()
{
	SquadManager::Instance().attack();
}
void HypothalamusClass::stop()
{
	SquadManager::Instance().stop();
}

void HypothalamusClass::vocalCommand(std::string command)
{
	std::vector<std::string> commandParts;
	boost::split(commandParts, command, ::isspace);

	
	if(commandParts.size() >= 2)		// two+ part commands
	{
		if(commandParts[0] == "b" || commandParts[0] == "build")
			parseBuildCommand(commandParts[1]);
		else if(commandParts[0] == "t" || commandParts[0] == "train")
			parseTrainCommand(commandParts[1]);
		else if(commandParts[0] == "o" || commandParts[0] == "order")
			parseOrderCommand(commandParts[1]);
		else if(commandParts[0] == "a" || commandParts[0] == "army")
			parseArmyBehaviourCommand(commandParts[1]);
		else if(commandParts[0] == "p" || commandParts[0] == "produce")
			parseProductionCommand(commandParts[1]);
		else if(commandParts[0] == "u" || commandParts[0] == "upgrade")
			parseUpgradeCommand(commandParts[1]);

	}else if(commandParts.size() == 1)	// one part command
	{
		if(commandParts[0] == "debug")
			EQUEUE( new ToggleDebugInfoEvent() );
		else if(commandParts[0] == "attack")
			attack();
		else if(commandParts[0] == "stop")
			stop();
	}
}

void HypothalamusClass::parseBuildCommand(std::string building)
{
	BWAPI::UnitType type;
	
	if(building == "tribunal")
		type = BWAPI::UnitTypes::Protoss_Arbiter_Tribunal;
	else if(building == "assimilator")
		type = BWAPI::UnitTypes::Protoss_Assimilator;
	else if(building == "citadel")
		type = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
	else if(building == "cybercore")
		type = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	else if(building == "beacon")
		type = BWAPI::UnitTypes::Protoss_Fleet_Beacon;
	else if(building == "forge")
		type = BWAPI::UnitTypes::Protoss_Forge;
	else if(building == "gate")
		type = BWAPI::UnitTypes::Protoss_Gateway;
	else if(building == "nexus")
		type = BWAPI::UnitTypes::Protoss_Nexus;
	else if(building == "observatory")
		type = BWAPI::UnitTypes::Protoss_Observatory;
	else if(building == "cannon")
		type = BWAPI::UnitTypes::Protoss_Photon_Cannon;
	else if(building == "pylon")
		type = BWAPI::UnitTypes::Protoss_Pylon;
	else if(building == "robo")
		type = BWAPI::UnitTypes::Protoss_Robotics_Facility;
	else if(building == "battery")
		type = BWAPI::UnitTypes::Protoss_Shield_Battery;
	else if(building == "stargate")
		type = BWAPI::UnitTypes::Protoss_Stargate;
	else if(building == "archives")
		type = BWAPI::UnitTypes::Protoss_Templar_Archives;
	else if(building == "supportbay")
		type = BWAPI::UnitTypes::Protoss_Robotics_Support_Bay;
	else
		return;

	buildUnit(type, 1);
}
void HypothalamusClass::parseTrainCommand(std::string unit)
{
	BWAPI::UnitType type;
	
	if(unit == "arbiter")
		type = BWAPI::UnitTypes::Protoss_Arbiter;
	else if(unit == "carrier")
		type = BWAPI::UnitTypes::Protoss_Carrier;
	else if(unit == "corsair")
		type = BWAPI::UnitTypes::Protoss_Corsair;
	else if(unit == "darktemplar")
		type = BWAPI::UnitTypes::Protoss_Dark_Templar;
	else if(unit == "dragoon")
		type = BWAPI::UnitTypes::Protoss_Dragoon;
	else if(unit == "hightemplar")
		type = BWAPI::UnitTypes::Protoss_High_Templar;
	else if(unit == "interceptor")
		type = BWAPI::UnitTypes::Protoss_Interceptor;
	else if(unit == "observer")
		type = BWAPI::UnitTypes::Protoss_Observer;
	else if(unit == "probe" || unit == "worker")
		type = BWAPI::UnitTypes::Protoss_Probe;
	else if(unit == "reaver")
		type = BWAPI::UnitTypes::Protoss_Reaver;
	else if(unit == "scout")
		type = BWAPI::UnitTypes::Protoss_Scout;
	else if(unit == "shuttle")
		type = BWAPI::UnitTypes::Protoss_Shuttle;
	else if(unit == "zealot")
		type = BWAPI::UnitTypes::Protoss_Zealot;
	else
		return;

	buildUnit(type, 1);
}
void HypothalamusClass::parseOrderCommand(std::string order)
{
	Order orderType;
	
	if(order == "trainworkers")
		orderType = Order::TrainWorkers;
	else if(order == "scout")
		orderType = Order::Scout;
	else if(order == "supply")
		orderType = Order::SupplyManager;
	else if(order == "expand")
		orderType = Order::ExpansionManager;
	else if(order == "army")
		orderType = Order::MacroArmyProduction;
	else if(order == "tech")
		orderType = Order::MacroCanTech;
	else if(order == "production")
		orderType = Order::MacroProductionFacilities;
	else
		return;

	toggleOrder(orderType);
}
void HypothalamusClass::parseArmyBehaviourCommand(std::string armyBehaviour)
{
	ArmyBehaviour behave;

	
	if(armyBehaviour == "aggressive")
		behave = ArmyBehaviour::Aggresive;
	else if(armyBehaviour == "allin")
		behave = ArmyBehaviour::AllIn;
	else if(armyBehaviour == "defensive")
		behave = ArmyBehaviour::Defensive;
	else if(armyBehaviour == "contain")
		behave = ArmyBehaviour::Contain;
	else if(armyBehaviour == "default")
		behave = ArmyBehaviour::Default;
	else
		return;

	setArmyBehaviour(behave);
}
void HypothalamusClass::parseProductionCommand(std::string unit)
{
	BWAPI::UnitType type;
	
	if(unit == "arbiter")
		type = BWAPI::UnitTypes::Protoss_Arbiter;
	else if(unit == "carrier")
		type = BWAPI::UnitTypes::Protoss_Carrier;
	else if(unit == "corsair")
		type = BWAPI::UnitTypes::Protoss_Corsair;
	else if(unit == "darktemplar")
		type = BWAPI::UnitTypes::Protoss_Dark_Templar;
	else if(unit == "dragoon")
		type = BWAPI::UnitTypes::Protoss_Dragoon;
	else if(unit == "hightemplar")
		type = BWAPI::UnitTypes::Protoss_High_Templar;
	else if(unit == "interceptor")
		type = BWAPI::UnitTypes::Protoss_Interceptor;
	else if(unit == "observer")
		type = BWAPI::UnitTypes::Protoss_Observer;
	else if(unit == "probe" || unit == "worker")
		type = BWAPI::UnitTypes::Protoss_Probe;
	else if(unit == "reaver")
		type = BWAPI::UnitTypes::Protoss_Reaver;
	else if(unit == "scout")
		type = BWAPI::UnitTypes::Protoss_Scout;
	else if(unit == "shuttle")
		type = BWAPI::UnitTypes::Protoss_Shuttle;
	else if(unit == "zealot")
		type = BWAPI::UnitTypes::Protoss_Zealot;
	else
		return;

	addProduction(type);
}
void HypothalamusClass::parseUpgradeCommand(std::string upgradeCommand)
{
	
	BWAPI::UpgradeType type;
	
	if(upgradeCommand == "aira")
		type = BWAPI::UpgradeTypes::Protoss_Air_Armor;
	else if(upgradeCommand == "airw")
		type = BWAPI::UpgradeTypes::Protoss_Air_Weapons;
	else if(upgradeCommand == "grounda")
		type = BWAPI::UpgradeTypes::Protoss_Ground_Armor;
	else if(upgradeCommand == "groundw")
		type = BWAPI::UpgradeTypes::Protoss_Ground_Weapons;
	else if(upgradeCommand == "shields")
		type = BWAPI::UpgradeTypes::Protoss_Plasma_Shields;
	else
		return;
	

	upgrade(type, 3);
}