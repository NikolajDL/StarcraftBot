#include "GenericEvents.h"

const EventType DebugStringEvent::sk_EventType( "debug_string" );
const EventType UnitDiscoveredEvent::sk_EventType( "unit_discovered" );
const EventType UnitDestroyedEvent::sk_EventType( "unit_destroyed" );
const EventType AnalyseTerrainAndBasesEvent::sk_EventType( "analyse_terrain_and_bases" );
const EventType OnStartEvent::sk_EventType( "ai_starting" );
const EventType OnUpdateEvent::sk_EventType( "ai_updating" );
const EventType ChangeBuildOrderEvent::sk_EventType( "change_build_order" );
const EventType PauseBuildOrderEvent::sk_EventType( "build_order_paused" );
const EventType ContinueBuildOrderEvent::sk_EventType( "build_order_continued" );