#include "FrameworkPCH.h"


CollisionSystem::CollisionSystem()
{
}


CollisionSystem::~CollisionSystem()
{
    for (Collider* col : m_colliders)
    {
        delete col;
    }
    m_colliders.clear();
}

void CollisionSystem::CheckCollisions(GameCore* game) const
{
    // Collision detection
    for (unsigned int i = 0; i < m_colliders.size(); ++i)
    {
		// Collider 1
		Collider* collider1 = m_colliders[i];

		// skip if the game object is inactive
		if (!collider1->GetGameObject()->IsActive())
			continue;

        for (unsigned int j = i + 1; j < m_colliders.size(); ++j)
        {
            // bool to figure out whether or not we collided
            bool didCollide = false;

            // Collider pointers
            Collider* collider2 = m_colliders[j];

			// Skip if the game object is inactive
			if (!collider2->GetGameObject()->IsActive())
				continue;

            // Check Collider type
            ColliderType::ColliderType colliderType1 = collider1->GetColliderType();
            ColliderType::ColliderType colliderType2 = collider2->GetColliderType();

            // Call appropriate collider collision checker
            if (colliderType1 == ColliderType::Circle || colliderType2 == ColliderType::Circle)
            {
                // Circle to blank collision
                Collider* circle1 = (colliderType1 == ColliderType::Circle ? collider1 : collider2);
                Collider* otherCollider = (circle1 == collider1 ? collider2 : collider1);

                switch (otherCollider->GetColliderType())
                {
                case ColliderType::Circle:
                    didCollide = CircleToCircleCollision((CircleCollider*)circle1, (CircleCollider*)otherCollider);
                    break;
                default:
                    break;
                }
            }


            // Get response and do whatever
            if (didCollide)
            {
                CollisionEvent* ev = new CollisionEvent(collider1->GetGameObject(), collider2->GetGameObject());
                game->AddEvent(ev);
            }
        }
    }
}

void CollisionSystem::AddCircleColliderToWorld(const GameObject* parentObject)
{
    assert(parentObject != nullptr);

    Collider* collider = new CircleCollider(parentObject);
    m_colliders.push_back(collider);
}

bool CollisionSystem::RemoveCircleColliderFromWorld(const GameObject* const parentObject)
{
	assert(parentObject != nullptr);

	for (auto colIt = m_colliders.begin(); colIt != m_colliders.end(); ++colIt)
	{
		if ((*colIt)->GetGameObject() == parentObject)
		{
			m_colliders.erase(colIt);
			return true;
		}
	}

	return false;
}

bool CollisionSystem::CircleToCircleCollision(CircleCollider* circle1, CircleCollider* circle2) const
{
	assert(circle1->GetGameObject()->IsActive());

	// Can only collide if both game objects are active
	const GameObject* gObj1 = circle1->GetGameObject();
	const GameObject* gObj2 = circle2->GetGameObject();

	// Compare sizes and locations to see if they intersect
    vec2 distanceVec = circle1->GetGameObject()->GetPosition() - circle2->GetGameObject()->GetPosition();
    float sumOfSizes = circle1->GetSize() + circle2->GetSize();

    if (distanceVec.Length() < sumOfSizes)
    {
        return true;
    }

    return false;
}