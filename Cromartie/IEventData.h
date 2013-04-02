#pragma once

#include <strstream>

#include "Interface.h"
#include "HashedString.h"

typedef HashedString EventType;

class IEventData
{
public:
	virtual const EventType & GetEventType( void ) const = 0;
	virtual const std::string ToString( void ) const = 0;
	virtual long GetFramecount() const = 0;
};
typedef std::tr1::shared_ptr<IEventData> IEventDataPtr;