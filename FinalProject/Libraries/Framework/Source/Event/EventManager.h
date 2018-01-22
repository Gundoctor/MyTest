#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include <list>

class Event;
class GameCore;

class EventManager
{
private:
    std::list<Event *> m_events;

public:
    EventManager();
    ~EventManager();

    void ProcessEvents(GameCore* pGameCore);
    void PushEvent(Event* pEvent);

    void ClearAllEvents();
};


#endif