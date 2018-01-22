#include "FrameworkPCH.h"

CollisionEvent::CollisionEvent(const GameObject* object1, const GameObject* object2) :
    Event(EventType::Collision),
    m_object1(object1),
    m_object2(object2)
{

}

CollisionEvent::~CollisionEvent()
{
}

// Getters
const GameObject* CollisionEvent::GetObject1() const
{
    return m_object1;
}

const GameObject* CollisionEvent::GetObject2() const
{
    return m_object2;
}