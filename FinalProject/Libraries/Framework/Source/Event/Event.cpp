#include "FrameworkPCH.h"
#include "Event.h"


Event::Event(EventType::EventType type) :
    m_type(type)
{
}

Event::~Event()
{
}

EventType::EventType Event::GetEventType()
{
    return m_type;
}