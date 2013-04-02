#include "EventManager.h"


#include <assert.h>


EventManagerClass::EventManagerClass()
{
    m_activeQueue = 0;
}


EventManagerClass::~EventManagerClass()
{
    //
}


bool EventManagerClass::AddListener(const EventListenerDelegate& eventDelegate, const EventType& type)
{

	EventListenerList& eventListenerList = (type==CatchAllEvents ? m_allEventsListerners : m_eventListeners[type]);

	// Check for duplicate
	for (EventListenerList::iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
	{
		if (eventDelegate == (*it))
		{
			return false;
		}
	}
	// No duplicates - add it.
	eventListenerList.push_back(eventDelegate);

	return true;
}

bool EventManagerClass::AddListener(const EventListenerDelegate0& eventDelegate, const EventType& type)
{

	EventListenerList0& eventListenerList = (type==CatchAllEvents ? m_allEventsListerners0 : m_eventListeners0[type]);

	// Check for duplicate
	for (EventListenerList0::iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
	{
		if (eventDelegate == (*it))
		{
			return false;
		}
	}
	// No duplicates - add it.
	eventListenerList.push_back(eventDelegate);

	return true;
}

bool EventManagerClass::RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type)
{
	bool success = false;

	// check of the eventtype is registered
	EventListenerMap::iterator findIt = m_eventListeners.find(type);
	if(findIt != m_eventListeners.end())	// We've found something yo
	{
		// Go through each listener with the given eventtype. 
		EventListenerList& listeners = findIt->second;
		for (EventListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
		{
			// if we find the one we're looking for - delete and break out of loop.
			if(eventDelegate == (*it))
			{
				listeners.erase(it);
				success = true; // WOO
				break;	// We assume here that only one listener registered to the given eventtype is found - so we can get the heck out.
			}
		}
	}

	return success;
}
bool EventManagerClass::RemoveListener(const EventListenerDelegate0& eventDelegate, const EventType& type)
{
	bool success = false;

	// check of the eventtype is registered
	EventListenerMap0::iterator findIt = m_eventListeners0.find(type);
	if(findIt != m_eventListeners0.end())	// We've found something yo
	{
		// Go through each listener with the given eventtype. 
		EventListenerList0& listeners = findIt->second;
		for (EventListenerList0::iterator it = listeners.begin(); it != listeners.end(); ++it)
		{
			// if we find the one we're looking for - delete and break out of loop.
			if(eventDelegate == (*it))
			{
				listeners.erase(it);
				success = true; // WOO
				break;	// We assume here that only one listener registered to the given eventtype is found - so we can get the heck out.
			}
		}
	}

	return success;
}


bool EventManagerClass::TriggerEvent(const IEventDataPtr& pEvent) const
{
	bool processed = false;

	// First trigger the catch-all listeners
	for (EventListenerList::const_iterator it = m_allEventsListerners.begin(); it != m_allEventsListerners.end(); ++it)
    {
        EventListenerDelegate listener = (*it);
		listener(pEvent);  // invoke the delegate
		processed = true;
    }
	// First trigger the catch-all 0 listeners
	for (EventListenerList0::const_iterator it = m_allEventsListerners0.begin(); it != m_allEventsListerners0.end(); ++it)
    {
        EventListenerDelegate0 listener = (*it);
		listener();  // invoke the delegate
		processed = true;
    }

	// As usual, see if we can find the eventtype
	EventListenerMap::const_iterator findIt = m_eventListeners.find(pEvent->GetEventType());
    if (findIt != m_eventListeners.end())
    {
		// We've found something. Now go through each listener invoking the delegate as we go.
		const EventListenerList& eventListenerList = findIt->second;
        for (EventListenerList::const_iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
        {
            EventListenerDelegate listener = (*it);
			listener(pEvent);  // invoke the delegate
			processed = true;
        }
	}
	// As usual, see if we can find the eventtype
	EventListenerMap0::const_iterator findIt0 = m_eventListeners0.find(pEvent->GetEventType());
    if (findIt0 != m_eventListeners0.end())
    {
		// We've found something. Now go through each listener invoking the delegate as we go.
		const EventListenerList0& eventListenerList = findIt0->second;
        for (EventListenerList0::const_iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
        {
            EventListenerDelegate0 listener = (*it);
			listener();  // invoke the delegate
			processed = true;
        }
	}

	return processed;
}


bool EventManagerClass::QueueEvent(const IEventDataPtr& pEvent)
{
    assert(m_activeQueue >= 0);
    assert(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

    // make sure the event is valid
    if (!pEvent)
    {
        return false;
    }

	// You know the drill - go fetch
	EventListenerMap::iterator findIt = m_eventListeners.find(pEvent->GetEventType());
	EventListenerList::const_iterator findAll = m_allEventsListerners.begin();
	EventListenerMap0::iterator findIt0 = m_eventListeners0.find(pEvent->GetEventType());
	EventListenerList0::const_iterator findAll0 = m_allEventsListerners0.begin();
	if (findIt != m_eventListeners.end() || findAll != m_allEventsListerners.end()
		|| findIt0 != m_eventListeners0.end() || findAll0 != m_allEventsListerners0.end())
    {
		// Add to active queue, maayn
        m_queues[m_activeQueue].push_back(pEvent);
        return true;
    }
    else
    {
        return false;
    }
}


bool EventManagerClass::AbortEvent(const EventType& inType, bool allOfType)
{
    assert(m_activeQueue >= 0);
    assert(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

    bool success = false;

	// Go fish
    EventListenerMap::iterator findIt = m_eventListeners.find(inType);
    EventListenerMap0::iterator findIt0 = m_eventListeners0.find(inType);
    if (findIt != m_eventListeners.end() || findIt0 != m_eventListeners0.end())
    {
		// Get active queue
        EventQueue& eventQueue = m_queues[m_activeQueue];

		// Go through it.
		EventQueue::iterator it = eventQueue.begin();
        while (it != eventQueue.end())
        {
            // Removing an item from the queue will invalidate the iterator, so have it point to the next member.  All
            // work inside this loop will be done using thisIt.
            EventQueue::iterator thisIt = it;
            ++it;

			// Remove event from queue if it is in it.
            if ((*thisIt)->GetEventType() == inType)
            {
                eventQueue.erase(thisIt);
                success = true;
                if (!allOfType) break;	// Break if we're only allowed to erase one instance of the event.
            }
        }
    }

    return success;
}


bool EventManagerClass::update(unsigned long maxMillis)
{
	unsigned long currMs = BWAPI::Broodwar->getFrameCount();
    unsigned long maxMs = ((maxMillis == EventManagerClass::kINFINITE) ? (EventManagerClass::kINFINITE) : (currMs + maxMillis));

    // swap active queues and clear the new queue after the swap
	int queueToProcess = m_activeQueue;
    m_activeQueue = (m_activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
    m_queues[m_activeQueue].clear();

    // Process the queue
    while (!m_queues[queueToProcess].empty())
    {
		// pop the front of the queue
        IEventDataPtr pEvent = m_queues[queueToProcess].front();
		m_queues[queueToProcess].pop_front();

        const EventType& eventType = pEvent->GetEventType();

		// trigger the catch-all listerners
		for (EventListenerList::const_iterator it = m_allEventsListerners.begin(); it != m_allEventsListerners.end(); ++it)
		{
			EventListenerDelegate listener = (*it);
			listener(pEvent);  // invoke the delegate
		}
		// trigger the catch-all listerners
		for (EventListenerList0::const_iterator it = m_allEventsListerners0.begin(); it != m_allEventsListerners0.end(); ++it)
		{
			EventListenerDelegate0 listener = (*it);
			listener();  // invoke the delegate
		}

		// find all the delegate functions registered for this event
        EventListenerMap::iterator findIt = m_eventListeners.find(eventType);
        if (findIt != m_eventListeners.end())
        {
            const EventListenerList& eventListeners = findIt->second;

			// call each listener
			for (EventListenerList::const_iterator it = eventListeners.begin(); it != eventListeners.end(); ++it)
            {
				EventListenerDelegate listener = (*it);
                listener(pEvent);
            }
        }
		
		// find all the delegate functions registered for this event
        EventListenerMap0::iterator findIt0 = m_eventListeners0.find(eventType);
        if (findIt0 != m_eventListeners0.end())
        {
            const EventListenerList0& eventListeners = findIt0->second;

			// call each listener
			for (EventListenerList0::const_iterator it = eventListeners.begin(); it != eventListeners.end(); ++it)
            {
				EventListenerDelegate0 listener = (*it);
                listener();
            }
        }

		// check to see if time ran out
        currMs = BWAPI::Broodwar->getFrameCount();
        if (maxMillis != EventManagerClass::kINFINITE && currMs >= maxMs)
		{
            break;
		}
    }
        
    // If we couldn't process all of the events, push the remaining events to the new active queue.
    // Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
    bool queueFlushed = (m_queues[queueToProcess].empty());
    if (!queueFlushed)
    {
        while (!m_queues[queueToProcess].empty())
        {
            IEventDataPtr pEvent = m_queues[queueToProcess].back();
            m_queues[queueToProcess].pop_back();
            m_queues[m_activeQueue].push_front(pEvent);
        }
    }
        
    return queueFlushed;
}