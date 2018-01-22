#ifndef __EVENT_H__
#define __EVENT_H__

namespace EventType
{
    enum EventType
    {
        Input,
        Game,
        Collision,
        Misc,
        NotSet
    };
}

class Event
{
public:
    Event(EventType::EventType type = EventType::NotSet);
    virtual ~Event() = 0;

    EventType::EventType GetEventType();

private:
    EventType::EventType m_type;
};

#endif