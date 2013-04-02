#pragma once

#include "IEventData.h"

#include <string>
#include <sstream>
#include <fstream>

class EventRecorder
{
public: 
	explicit EventRecorder( char const * const kpLogFileName = NULL); 
	~EventRecorder(); 
	
	void RecordEvent(IEventDataPtr pEventData); 

private: 
	std::ofstream *m_outputStream;
};