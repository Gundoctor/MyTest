#ifndef __COLLIDER_H__
#define __COLLIDER_H__

class GameObject;

namespace ColliderType
{
    enum ColliderType : unsigned int
    {
        Circle,

    };
}

class Collider
{
public:
    Collider(ColliderType::ColliderType type, const GameObject* gObj);
    virtual ~Collider();

    // GETTERS
    const GameObject* GetGameObject() const;
    ColliderType::ColliderType GetColliderType() const;

protected:
    ColliderType::ColliderType m_type;
    const GameObject* m_gObj;

};


#endif