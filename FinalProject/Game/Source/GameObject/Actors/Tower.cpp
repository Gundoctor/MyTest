#include "GamePCH.h"


Tower::Tower(TilemapScene* pScene, vec2 pos, vec2 scale, Mesh* pMesh, GLuint texID, SpriteFormat sprite) :
    GameObject(GameObjectType::Tower, pos, scale),
	m_pTilemapScene(pScene),
	m_pAIStateMethod(&Tower::AIState_Idle),
	m_pPrevAIStateMethod(&Tower::AIState_Idle),
	m_reloadTimer(RELOAD_TIME),
	m_timeToShoot(1.0f / RATE_OF_FIRE),
	m_shotsLeft(SHOT_COUNT),
	m_pTarget(nullptr),
	m_spriteInfo(sprite),
	m_TexID(texID)
{
	SetMesh(pMesh);
}

Tower::Tower(TilemapScene* pScene, SceneNode* parent, vec2 pos, vec2 scale, Mesh* pMesh, GLuint texID, SpriteFormat sprite) :
    GameObject(GameObjectType::Tower, parent, pos, scale),
	m_pTilemapScene(pScene),
	m_pAIStateMethod(&Tower::AIState_Idle),
	m_pPrevAIStateMethod(&Tower::AIState_Idle),
	m_reloadTimer(RELOAD_TIME),
	m_timeToShoot(1.0f / RATE_OF_FIRE),
	m_shotsLeft(SHOT_COUNT),
	m_pTarget(nullptr),
	m_spriteInfo(sprite),
	m_TexID(texID)
{
	SetMesh(pMesh);
}

Tower::~Tower()
{

}

float Tower::GetSize() const
{
	return TOWER_RANGE;
}

void Tower::UpdateCurrent(float deltatime)
{
	if (IsActive())
		(this->*m_pAIStateMethod)(deltatime);
}

void Tower::DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 camProj, ShaderProgram* camShader)
{
	if (IsActive())
	{
		assert(m_pShader != nullptr);
		assert(m_pMesh != nullptr);

		if (camShader != nullptr)
		{
			m_pMesh->DrawSprite(m_scale, worldPos, camPos, camProj, camShader, m_TexID, m_spriteInfo);
		}
		else
		{
			m_pMesh->DrawSprite(m_scale, worldPos, camPos, camProj, m_pShader, m_TexID, m_spriteInfo);
		}
	}
}

void Tower::ChangeAIState(AIStateMethod pMethod)
{
	m_pPrevAIStateMethod = m_pAIStateMethod;
	m_pAIStateMethod = pMethod;
}

void Tower::AIState_Idle(float deltatime)
{
	// Check if there's an enemy nearby
	// Returns nullptr if there are no ACTIVE enemies nearby
	m_pTarget = m_pTilemapScene->FindClosestEnemy(this);

	if (m_pTarget != nullptr)
	{
		ChangeAIState(&Tower::AIState_Attack);
	}

	// Check if we are less than max bullets
	if (m_shotsLeft < SHOT_COUNT)
	{
		ChangeAIState(&Tower::AIState_Reload);
	}
}

void Tower::AIState_Reload(float deltatime)
{
	// Reloading
	// Toggle reload boolean to on
	// So long as reload is on, AIState remains in Reload
	// After enough time has passed (ie reload timer = 0)
	// Set state back to wherever we came from

	m_reloadTimer -= deltatime;
	if (m_reloadTimer <= 0.0f)
	{
		m_shotsLeft = SHOT_COUNT;
		m_reloadTimer = RELOAD_TIME;
		ChangeAIState(m_pPrevAIStateMethod);
	}
}

void Tower::AIState_Attack(float deltatime)
{
	// Attacking
	// Enemy comes into range of the tower
	// So long as the enemy is in range, tower will fire at the enemy
	// If the enemy is now out of range, ask the scene to calculate the closest enemy
	// in range of the tower. That enemy becomes the new target.
	// If no enemies in range, go to idle

	// Make sure we have a valid target
	if (m_pTarget == nullptr)
	{
		ChangeAIState(&Tower::AIState_Idle);
		return;
	}

	// Make sure target is still active
	else if (!m_pTarget->IsActive())
	{
		ChangeAIState(&Tower::AIState_Idle);
		return;
	}

	// Check if we can shoot
	m_timeToShoot -= deltatime;
	if (m_timeToShoot > 0.0f)
	{
		return;
	}
	else
	{
		// Reset shoot timer
		m_timeToShoot += 1.0f / RATE_OF_FIRE;
	}

	// Check if we need reload
	if (m_shotsLeft <= 0)
	{
		ChangeAIState(&Tower::AIState_Reload);
		return;
	}


	// Check if target it in range
	float range = GetSize();
	vec2 displacement = m_pTarget->GetWorldPosition() - GetWorldPosition();
	if (displacement.LengthSquared() <= range * range)
	{
		Bullet* bullet = m_pTilemapScene->GetInactiveBullet();

		if (bullet != nullptr)
		{
			bullet->SetPosition(GetPosition());
			bullet->SetVelocity(displacement.GetNormalized());
			bullet->Activate();

			m_shotsLeft -= 1;
		}
	}
	else
	{
		// Enemy out of range, find another enemy
		m_pTarget = m_pTilemapScene->FindClosestEnemy(this);
	}
}