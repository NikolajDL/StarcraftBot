#pragma once

#include "IEventData.h"

class BaseEventData : public IEventData
{
public:
    explicit BaseEventData( const long framecount = 0 )
            : m_Framecount( framecount )
    {
    }

    virtual ~BaseEventData() { }

    virtual const EventType & GetEventType( void ) const = 0;
    virtual const std::string ToString( void ) const 
	{
		return GetEventType().getStr();
	}

    long GetFramecount( void ) const
    {
            return m_Framecount;
    }

protected:
    const long m_Framecount;
};