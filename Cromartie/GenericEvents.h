#pragma once

#include <boost/lexical_cast.hpp>

#include "Interface.h"
#include "StringBuilder.h"
#include "BaseEventData.h"
#include "Region.h"
#include "BuildOrder.h"

struct DebugStringEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }

    enum DebugStringType
    {
        Info,
		Debug,
		Warning,
		Error,
		Fatal
    };

	const char * getTextForEnum( int enumVal ) const
	{
		switch( enumVal )
		{
		case DebugStringType::Info:
			return "Info";
		case DebugStringType::Debug:
			return "Debug";
		case DebugStringType::Warning:
			return "Warning" ;
		case DebugStringType::Error:
			return "Error" ;
		case DebugStringType::Fatal:
			return "Fatal" ;
		default:
			return "NA";
		}
	}
	
	virtual const std::string ToString( void ) const
    {
		return "[" + std::string(getTextForEnum(m_Type)) + "] - " + m_DebugMessage;
    }

    explicit DebugStringEvent( const std::string & debugMessage, const DebugStringType type )
        : m_DebugMessage( debugMessage )
        , m_Type( type )
		, BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }

    virtual void serialize( std::ostrstream & out )
    {
        assert( 0 && "You should not be serializing debug strings!" );
    }

    std::string m_DebugMessage;
    DebugStringType m_Type;
};

struct UnitDiscoveredEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Unit Discovered. Type: " << m_Unit->getType().c_str() << " ID: " 
			<< boost::lexical_cast<std::string>(m_Unit->getID()).c_str();
    }

	explicit UnitDiscoveredEvent( BWAPI::Unit* unit)
        : m_Unit( unit )
		, BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
	
	BWAPI::Unit* m_Unit;
};
struct UnitCompleteEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Unit Complete. Type: " << m_Unit->getType().c_str() << " ID: " 
			<< boost::lexical_cast<std::string>(m_Unit->getID()).c_str();
    }

	explicit UnitCompleteEvent( BWAPI::Unit* unit)
        : m_Unit( unit )
		, BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
	
	BWAPI::Unit* m_Unit;
};

struct UnitDestroyedEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Unit Destroyed. Type: " << m_Unit->getType().c_str() << " ID: " 
			<< boost::lexical_cast<std::string>(m_Unit->getID()).c_str();
    }

	explicit UnitDestroyedEvent( BWAPI::Unit* unit)
        : m_Unit( unit )
		, BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
	
	BWAPI::Unit* m_Unit;
};
struct AnalyseTerrainAndBasesEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Analysing terrain and bases";
    }

	explicit AnalyseTerrainAndBasesEvent( )
        : BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
};
struct OnStartEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Cromartie is powering up!";
    }

	explicit OnStartEvent( )
        : BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
};
struct OnUpdateEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Cromartie updated!";
    }

	explicit OnUpdateEvent( )
        : BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
};
struct ChangeBuildOrderEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Change build. Build order: " << BuildOrderIDStrings[mBuildOrder.underlying()];
    }

	explicit ChangeBuildOrderEvent( BuildOrderID buildOrder)
        : mBuildOrder( buildOrder )
		, BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
	
	BuildOrderID mBuildOrder;
};
struct AddBuildOrderEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Add build. Build order: " << mBuildOrder->getName();
    }

	explicit AddBuildOrderEvent( BuildOrder *buildOrder)
        : mBuildOrder( buildOrder )
		, BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
	
	BuildOrder* mBuildOrder;
};
struct PauseBuildOrderEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Build Order paused!";
    }

	explicit PauseBuildOrderEvent( )
        : BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
};
struct ContinueBuildOrderEvent : public BaseEventData
{
    static const EventType sk_EventType;
    virtual const EventType & GetEventType( void ) const
    {
        return sk_EventType;
    }
	
	virtual const std::string ToString( void ) const
    {
		return StringBuilder() << "Build Order continued!";
    }

	explicit ContinueBuildOrderEvent( )
        : BaseEventData(BWAPI::Broodwar->getFrameCount())
    {
    }
};