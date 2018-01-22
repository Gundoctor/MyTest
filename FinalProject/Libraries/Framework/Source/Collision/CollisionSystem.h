#ifndef __PHYSICSENGINE_H__
#define __PHYSICSENGINE_H__

class Collider;
class GameCore;

class CollisionSystem
{
public:
    CollisionSystem();
    ~CollisionSystem();
    
    void CheckCollisions(GameCore* game) const;
    void AddCircleColliderToWorld(const GameObject* parentObject);
	bool RemoveCircleColliderFromWorld(const GameObject * const parentObject);
    //Collider* AddCircleColliderToWorld(GameObject* parentObject);


    bool CircleToCircleCollision(CircleCollider* collider1, CircleCollider* collider2) const;

private:
    std::vector<Collider*> m_colliders;
};

#endif