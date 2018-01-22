#include "FrameworkPCH.h"
#include "EventManager.h"

EventManager::EventManager()
{

}

EventManager::~EventManager()
{
    ClearAllEvents();
}

void EventManager::ProcessEvents(GameCore *pGameCore)
{
    for (auto it = m_events.begin(); it != m_events.end(); ++it)
    {
        pGameCore->ProcessEvent(*it);
    }

    ClearAllEvents();
}

void EventManager::PushEvent(Event* ev)
{
    m_events.push_back(ev);
}

void EventManager::ClearAllEvents()
{
    for (Event *ev : m_events)
    {
        delete ev;
    }
    m_events.clear();
}