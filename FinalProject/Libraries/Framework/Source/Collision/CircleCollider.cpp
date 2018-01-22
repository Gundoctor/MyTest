#include "FrameworkPCH.h"


CircleCollider::CircleCollider(const GameObject* gObj) :
    Collider(ColliderType::Circle, gObj)
{
    assert(gObj != nullptr);
}

CircleCollider::~CircleCollider()
{

}

float CircleCollider::GetSize() const
{
    return m_gObj->GetSize();
}