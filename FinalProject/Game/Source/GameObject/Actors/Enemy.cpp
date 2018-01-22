#include "GamePCH.h"


Enemy::Enemy(vec2 pos, vec2 scale, Mesh* pMesh, GLuint texID, TilemapScene* scene) :
    GameObject(GameObjectType::Enemy, pos, scale),
	m_TexID(texID),
	m_speed(ENEMY_SPEED),
	m_direction(0.0f, 1.0f),
	m_spriteTimer(ENEMY_SPRITE_TIMER),
	m_currSpriteIndex(0),
	m_scene(scene),
	m_target(pos)
{
	SetMesh(pMesh);
}

Enemy::Enemy(SceneNode* parent, vec2 pos, vec2 scale, Mesh* pMesh, GLuint texID, TilemapScene* scene) :
    GameObject(GameObjectType::Enemy, parent, pos, scale),
	m_TexID(texID),
	m_speed(ENEMY_SPEED),
	m_direction(0.0f, 1.0f),
	m_spriteTimer(ENEMY_SPRITE_TIMER),
	m_currSpriteIndex(0),
	m_scene(scene),
	m_target(pos)
{
	SetMesh(pMesh);
}

Enemy::~Enemy()
{

}

void Enemy::UpdateCurrent(float deltatime)
{
	if (!IsActive())
		return;

	if (HasReachedTarget())
	{
		bool res = UpdateTargetWithNewNode();

		// If we have reached the end, don't move
		if (!res)
		{
			m_speed = 0.0f;
		}
	}

	// Figure out the direction
	vec2 displacement = m_target - m_position;
	float distance = displacement.Length();
	m_direction = displacement.Normalize();
	


	// change spriteIndex
	m_spriteTimer -= deltatime;
	if (m_spriteTimer < 0.0f)
	{
		m_spriteTimer += ENEMY_SPRITE_TIMER;
		m_currSpriteIndex = (m_currSpriteIndex + 1) % 2;
	}

	float xMag = abs(m_direction.x);
	float yMag = abs(m_direction.y);

	// Going Right
	if (m_direction.x > 0.0f && xMag > yMag)
	{
		m_currSprite = m_spriteRight.at(m_currSpriteIndex);
	}
	// Going Left
	else if (m_direction.x < 0.0f && xMag > yMag)
	{
		m_currSprite = m_spriteLeft.at(m_currSpriteIndex);
	}

	// Going Up
	if (m_direction.y > 0.0f && yMag > xMag)
	{
		m_currSprite = m_spriteUp.at(m_currSpriteIndex);
	}
	// Going Down
	if (m_direction.y < 0.0f && yMag > xMag)
	{
		m_currSprite = m_spriteDown.at(m_currSpriteIndex);
	}

	// Change position
	displacement = m_direction * m_speed * deltatime;
	float nextStepDistanceSquared = displacement.LengthSquared();

	if (distance * distance < nextStepDistanceSquared)
	{
		displacement *= distance * distance / nextStepDistanceSquared;
	}

	m_position += displacement;

}

void Enemy::DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 camProj, ShaderProgram* pShader)
{
	assert(m_pMesh != nullptr);
	assert(m_pShader != nullptr);

	// Don't draw if inactive
	if (!IsActive())
		return;

	if (pShader != nullptr)
	{
		m_pMesh->DrawSprite(m_scale, worldPos, camPos, camProj, pShader, m_TexID, m_currSprite);
	}
	else
	{
		m_pMesh->DrawSprite(m_scale, worldPos, camPos, camProj, m_pShader, m_TexID, m_currSprite);
	}
}
void Enemy::Activate()
{
	GameObject::Activate();
	m_health = m_maxHealth;
	m_target = m_position;
}

void Enemy::SetDirection(vec2 dir)
{
	m_direction = dir;
}

void Enemy::SetSpeed(float speed)
{
	m_speed = speed;
}

void Enemy::InsertSpriteFormat(EnemyDir::EnemyDir dir, SpriteFormat sprite)
{
	switch (dir)
	{
	case EnemyDir::Left:
		m_spriteLeft.push_back(sprite);
		break;
	case EnemyDir::Right:
		m_spriteRight.push_back(sprite);
		break;
	case EnemyDir::Up:
		m_spriteUp.push_back(sprite);
		break;
	case EnemyDir::Down:
		m_spriteDown.push_back(sprite);
		break;
	}
}

bool Enemy::HasReachedTarget()
{
	vec2 displacement = m_target - m_position;

	if (displacement.LengthSquared() <= 1.0f)
	{
		return true;
	}

	return false;
}

bool Enemy::UpdateTargetWithNewNode()
{
	// Get access to needed stuff
	Pathfinder* pathfinder = m_scene->GetPathfinder();
	Tilemap* tilemap = m_scene->GetCurrentTilemap();


	// Check if we need to move at all
	vec2 endzone = m_scene->GetEndZone();
	vec2 endzoneWorld = tilemap->GetWorldPosAtTile(endzone);

	// Calculate where to go based on pathfinder
	vec2 startloc = tilemap->GetTileAtWorldPos(m_position);
	bool pathFound = pathfinder->FindPath((int)startloc.x, (int)startloc.y, (int)endzone.x, (int)endzone.y);

	// Assume path is found
	int path[255];
	int len = pathfinder->GetPath(path, 255, (int)endzone.x, (int)endzone.y);

	// Our current index is the last element
	// where to go next is the second to last element
	vec2 currentLocation(0.0f, 0.0f);
	Vector2Int mapSize = tilemap->GetMapSize();

	int nextLocIndex = -1;
	if (len == 0)
	{
		return false;
	}
	else if (len > 1)
	{
		int currIndex = path[len - 1];
		int tileX = currIndex % mapSize.x;
		int tileY = currIndex / mapSize.x;

		// need to flip the y
		currentLocation.x = (float)tileX;
		currentLocation.y = tilemap->GetInverseY((float)tileY);

		currentLocation = tilemap->GetWorldPosAtTile(currentLocation);


		// Check if we're roughly on where we should be
		if (len > 2)
			nextLocIndex = path[len - 2];
		else
			m_target = endzoneWorld;
	}

	// Calc tile position based on index
	if (nextLocIndex != -1)
	{
		int tileX = nextLocIndex % mapSize.x;
		int tileY = nextLocIndex / mapSize.x;

		// need to flip the y
		m_target.x = (float)tileX;
		m_target.y = tilemap->GetInverseY((float)tileY);

		m_target = tilemap->GetWorldPosAtTile(m_target);
	}

	return true;
}