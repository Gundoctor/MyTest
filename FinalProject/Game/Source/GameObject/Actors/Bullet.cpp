#include "GamePCH.h"

Bullet::Bullet(vec2 pos, vec2 scale, Mesh* pMesh) : 
	GameObject(GameObjectType::Bullet, pos, scale),
	m_velocity(0.0f, 0.0f),
	m_lifespan(BULLET_LIFE)
{
	SetMesh(pMesh);
}

Bullet::Bullet(SceneNode* parent, vec2 pos, vec2 scale, Mesh* pMesh) : 
	GameObject(GameObjectType::Bullet, parent, pos, scale),
	m_velocity(0.0f, 0.0f),
	m_lifespan(BULLET_LIFE)
{
	SetMesh(pMesh);
}

Bullet::~Bullet()
{

}

void Bullet::UpdateCurrent(float deltatime)
{
	if (IsActive())
	{
		m_lifespan -= deltatime;
		m_position += m_velocity * deltatime;

		if (m_lifespan < 0.0f)
		{
			Deactivate();
		}
	}
		
}

void Bullet::SetVelocity(vec2 dirVec)
{
	if (dirVec.LengthSquared() > 1)
		dirVec = dirVec.Normalize();

	m_velocity = dirVec * BULLET_SPEED;
}


void Bullet::Deactivate()
{
	GameObject::Deactivate();
	m_lifespan = BULLET_LIFE;
}