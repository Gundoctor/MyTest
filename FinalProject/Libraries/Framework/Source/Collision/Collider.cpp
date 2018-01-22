#include "FrameworkPCH.h"


Collider::Collider(ColliderType::ColliderType type, const GameObject* gObj) :
    m_type(type),
    m_gObj(gObj)
{
}

Collider::~Collider()
{
}

const GameObject* Collider::GetGameObject() const
{
    return m_gObj;
}

ColliderType::ColliderType Collider::GetColliderType() const
{
    return m_type;
}