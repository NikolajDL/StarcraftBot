#pragma once

#include "Singleton.h"
#include "IEventData.h"
#include "FastDelegate.h"

typedef std::tr1::shared_ptr<IEventData> IEventDataPtr;
typedef fastdelegate::FastDelegate0<> EventListenerDelegate0;
typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

// CatchAllEvent
const EventType CatchAllEvents = EventType("*");

class EventManagerClass
{
	typedef std::list<EventListenerDelegate0> EventListenerList0;
    typedef std::map<EventType, EventListenerList0> EventListenerMap0;
	typedef std::list<EventListenerDelegate> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::list<IEventDataPtr> EventQueue;
	
	EventListenerMap0 m_eventListeners0;
	EventListenerList0 m_allEventsListerners0;
	EventListenerMap m_eventListeners;
	EventListenerList m_allEventsListerners;
    EventQueue m_queues[EVENTMANAGER_NUM_QUEUES];
    int m_activeQueue;
public:
	enum Constants { kINFINITE = 0xffffffff };

	explicit EventManagerClass();
	virtual ~EventManagerClass(void);

	virtual bool AddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
	virtual bool AddListener(const EventListenerDelegate0& eventDelegate, const EventType& type);
    virtual bool RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);
    virtual bool RemoveListener(const EventListenerDelegate0& eventDelegate, const EventType& type);

    virtual bool TriggerEvent(const IEventDataPtr& pEvent) const;
    virtual bool QueueEvent(const IEventDataPtr& pEvent);
    virtual bool AbortEvent(const EventType& type, bool allOfType = false);

    virtual bool update(unsigned long maxMillis = kINFINITE);
};

typedef Singleton<EventManagerClass> EventManager;

#define ETRIGGER(evnt) EventManager::Instance().TriggerEvent(IEventDataPtr(evnt))
#define EQUEUE(evnt) EventManager::Instance().QueueEvent(IEventDataPtr(evnt))