#include "Cromartie.h"

#include <iostream>

#include "DrawBuffer.h"
#include "TerrainAnalysis.h"
#include "UnitTracker.h"
#include "BaseTracker.h"
#include "BuildingPlacer.h"
#include "TaskManager.h"
#include "ResourceTracker.h"
#include "ResourceManager.h"
#include "TrainTask.h"
#include "ConstructionTask.h"
#include "MorphTask.h"
#include "WallTracker.h"
#include "BuildOrderManager.h"
#include "PlayerTracker.h"
#include "BorderTracker.h"
#include "ScoutManager.h"
#include "SupplyManager.h"
#include "MacroManager.h"
#include "ExpansionManager.h"
#include "UnitInformation.h"
#include "UnitHelper.h"
#include "LatencyTracker.h"
#include "PylonPowerTracker.h"
#include "MapHelper.h"
#include "PathFinder.h"
#include "Logger.h"
#include "GameProgressDetection.h"
#include "AOEThreatTracker.h"
#include "SquadManager.h"
#include "BlockedPathManager.h"
#include "GameMemory.h"
#include "EventManager.h"
#include "GenericEvents.h"
#include "FastDelegate.h"
#include "Hypothalamus.h"

using namespace fastdelegate;


Cromartie::Cromartie()
	: mOnBegin(false)
	, mLeavingGame(0)
	, mSaidGG(false)
{
}

Cromartie::~Cromartie(void)
{
	delete eventRecorderListener;
}

void Cromartie::onStart()
{
	BWAPI::Broodwar->sendText("Cromartie 1.0 Operational");

	BWAPI::Broodwar->setLatCom(false);
	BWAPI::Broodwar->setCommandOptimizationLevel(1);

	if(BWAPI::Broodwar->self()->getRace() != BWAPI::Races::Protoss)
		BWAPI::Broodwar->sendText("Cromartie is a Protoss only bot");

	if(BWAPI::Broodwar->getRevision() != BWAPI::BWAPI_getRevision())
	{
		BWAPI::Broodwar->sendText("This version of Cromartie was compiled for BWAPI rev. %d", BWAPI::BWAPI_getRevision());
		BWAPI::Broodwar->sendText("But BWAPI rev. %d is currently running", BWAPI::Broodwar->getRevision());
	}

	BWAPI::Broodwar->setLocalSpeed(20);

	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

	BWAPI::Broodwar->sendText("Starting GA...");


	
	BWAPI::Broodwar->sendText("GA started!");
	
	registerListeners();
}

void Cromartie::onEnd(bool isWinner)
{
	BuildOrderManager::Instance().onEnd(isWinner);
	GameMemory::Instance().onEnd();
}

void Cromartie::onFrame()
{
	// Enqueue bwapi events
	for each(BWAPI::Event bwapiEvent in BWAPI::Broodwar->getEvents())
	{
		if(bwapiEvent.getType() == BWAPI::EventType::UnitDiscover)
			EQUEUE( new UnitDiscoveredEvent(bwapiEvent.getUnit()));
		if(bwapiEvent.getType() == BWAPI::EventType::UnitDestroy)
			EQUEUE( new UnitDestroyedEvent(bwapiEvent.getUnit()));
	}

	if(!mOnBegin)
	{
		BWAPI::Broodwar->sendText("Starting GA...");
		//_ga.onStarcraftStart();
		BWAPI::Broodwar->sendText("GA started!");
		mOnBegin = true;
		EQUEUE( new OnStartEvent() );
		EQUEUE( new PauseBuildOrderEvent() );
	}
	

	EQUEUE( new OnUpdateEvent() );
	EventManager::Instance().update();

	if(GameProgressDetection::Instance().shouldGG())
	{
		if(mLeavingGame == 0)
			mLeavingGame = BWAPI::Broodwar->getFrameCount();
	}
	else
		mLeavingGame = 0;

	if(mLeavingGame != 0)
	{
		if(BWAPI::Broodwar->getFrameCount() - mLeavingGame > 24 && !mSaidGG)
		{
			mSaidGG = true;
			BWAPI::Broodwar->sendText("gg");
		}
		else if(BWAPI::Broodwar->getFrameCount() - mLeavingGame > 80)
			BWAPI::Broodwar->leaveGame();
	}
}

void Cromartie::onSendText(std::string text) 
{
	Hypothalamus::Instance().vocalCommand(text);

	BWAPI::Broodwar->sendText(text.c_str());
}

void Cromartie::onPlayerLeft(Player player)
{
	PlayerTracker::Instance().onPlayerLeft(player);
}

#define ADDLISTENER(instance, functionPointer, eventType) EventManager::Instance().AddListener(MakeDelegate(instance, functionPointer), eventType);

void Cromartie::registerListeners()
{
	eventRecorderListener = new EventRecorder("bwapi-data\\logs\\Cromartie_EventLog.txt");
	ADDLISTENER(eventRecorderListener, &EventRecorder::RecordEvent, CatchAllEvents);

	ADDLISTENER(&UnitTracker::Instance(), &UnitTrackerClass::onUnitDiscoverEvent, UnitDiscoveredEvent::sk_EventType);
	ADDLISTENER(&UnitTracker::Instance(), &UnitTrackerClass::onUnitDestroyEvent, UnitDestroyedEvent::sk_EventType);
	
	ADDLISTENER(&WallTracker::Instance(), &WallTrackerClass::onBegin, OnStartEvent::sk_EventType);
	ADDLISTENER(&PlayerTracker::Instance(), &PlayerTrackerClass::onBegin, OnStartEvent::sk_EventType);
	ADDLISTENER(&TerrainAnalysis::Instance(), &TerrainAnalysisClass::analyseBasesAndTerrain, OnStartEvent::sk_EventType);
	ADDLISTENER(&BorderTracker::Instance(), &BorderTrackerClass::onBegin, OnStartEvent::sk_EventType);
	ADDLISTENER(&UnitTracker::Instance(), &UnitTrackerClass::pumpUnitEvents, OnStartEvent::sk_EventType);
	ADDLISTENER(&ScoutManager::Instance(), &ScoutManagerClass::onBegin, OnStartEvent::sk_EventType);
	ADDLISTENER(&MacroManager::Instance(), &MacroManagerClass::onBegin, OnStartEvent::sk_EventType);
	ADDLISTENER(&GameMemory::Instance(), &GameMemoryClass::onBegin, OnStartEvent::sk_EventType);
	ADDLISTENER(&BuildOrderManager::Instance(), &BuildOrderManagerClass::onBegin, OnStartEvent::sk_EventType);
	ADDLISTENER(&BlockedPathManager::Instance(), &BlockedPathManagerClass::onBegin, OnStartEvent::sk_EventType);
	
	ADDLISTENER(&UnitTracker::Instance(), &UnitTrackerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&AOEThreatTracker::Instance(), &AOEThreatTrackerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&BlockedPathManager::Instance(), &BlockedPathManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&PylonPowerTracker::Instance(), &PylonPowerTrackerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&UnitInformation::Instance(), &UnitInformationClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&BorderTracker::Instance(), &BorderTrackerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&PlayerTracker::Instance(), &PlayerTrackerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&BaseTracker::Instance(), &BaseTrackerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&BuildingPlacer::Instance(), &BuildingPlacerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&ResourceManager::Instance(), &ResourceManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&SupplyManager::Instance(), &SupplyManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&MacroManager::Instance(), &MacroManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&ExpansionManager::Instance(), &ExpansionManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&LatencyTracker::Instance(), &LatencyTrackerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&GameProgressDetection::Instance(), &GameProgressDetectionClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&BuildOrderManager::Instance(), &BuildOrderManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&ScoutManager::Instance(), &ScoutManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&SquadManager::Instance(), &SquadManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&TaskManager::Instance(), &TaskManagerClass::update, OnUpdateEvent::sk_EventType);
	ADDLISTENER(&DrawBuffer::Instance(), &DrawBufferClass::update, OnUpdateEvent::sk_EventType);
	
	ADDLISTENER(&BuildOrderManager::Instance(), &BuildOrderManagerClass::changeCurrentBuildEvent, ChangeBuildOrderEvent::sk_EventType);
	ADDLISTENER(&BuildOrderManager::Instance(), &BuildOrderManagerClass::pauseBuild, PauseBuildOrderEvent::sk_EventType);
	ADDLISTENER(&BuildOrderManager::Instance(), &BuildOrderManagerClass::continueBuild, ContinueBuildOrderEvent::sk_EventType);
}