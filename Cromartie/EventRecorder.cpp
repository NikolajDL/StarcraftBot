#include "EventRecorder.h"


EventRecorder::EventRecorder(char const * const kpLogFileName)
{
	m_outputStream = new std::ofstream(kpLogFileName, std::ios::ate);
}
EventRecorder::~EventRecorder()
{
	if((*m_outputStream).is_open())
		(*m_outputStream).close();
	delete m_outputStream;
}

void EventRecorder::RecordEvent(IEventDataPtr pEventData)
{
	*m_outputStream 
		<< "Event at frame " << (*pEventData).GetFramecount()  
		<< " : " 
		<< "Type: " << (*pEventData).GetEventType().getStr() 
		<< " : " 
		<< "Data: " << (*pEventData).ToString()
		<< std::endl;
}