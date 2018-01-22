#include "GamePCH.h"

Player::Player(vec2 pos, float size, GameSample* gameSample) :
    GameObject(GameObjectType::Player, pos, size),
	m_pGameSample(gameSample),
	m_pTilemap(nullptr)
{
    m_pController = new PlayerController();
}


Player::~Player()
{
    if (m_pController != nullptr)
    {
        delete m_pController;
        m_pController = nullptr;
    }
}

void Player::UpdateCurrent(float deltatime)
{
	unsigned int flags = m_pController->GetCurrentFlags();
    vec2 movementValues = m_pController->GetMovementValues();
	vec2 dirVec(0.0f, 0.0f);

	if (flags & PLAYER_LEFT)
	{
		dirVec.x -= PLAYER_SPEED;
	}
	if (flags & PLAYER_RIGHT)
	{
		dirVec.x += PLAYER_SPEED;
	}
	if (flags & PLAYER_UP)
	{
		dirVec.y += PLAYER_SPEED;
	}
	if (flags & PLAYER_DOWN)
	{
		dirVec.y -= PLAYER_SPEED;
	}

    // Normalize direction vector, and then multiply by speed
	dirVec.Normalize();
	vec2 displacement = dirVec * PLAYER_SPEED * deltatime;

    // Multiply by controller values
    displacement *= movementValues;

	if (m_pTilemap == nullptr)
	{
		// Check bounds of world
		vec2 preMovePos = m_position + displacement;
		vec2 gameWorldOrigin = m_pGameSample->GetWorldOrigin();
		vec2 gameWorldSize = m_pGameSample->GetWorldSize() / 2;
		float size = GetSize();

		// Center our position with regards to the world position
		// And then check to see if our position is within the world's "size"
		// Decrease the world's size by our size to account for our borders
		if (fabs(preMovePos.x - gameWorldOrigin.x) > (gameWorldSize.x - size))
		{
			// Calculate how far over the bound we are trying to move
			float dispX = fabs(preMovePos.x - gameWorldOrigin.x) - (gameWorldSize.x - size); // positive value

																							 // Decrease the displacement by how over we are trying to move
			displacement.x -= dispX * (displacement.x / fabs(displacement.x));
		}
		if (fabs(preMovePos.y - gameWorldOrigin.y) > (gameWorldSize.y - size))
		{
			// Calculate how far over the y bound we are trying to move
			float dispY = fabs(preMovePos.y - gameWorldOrigin.y) - (gameWorldSize.y - size); // positive value

																							 // Decrease the displacement y val by how far over we would be moving
			displacement.y -= dispY * (displacement.y / fabs(displacement.y));
		}
	}
	//else
	//{
	//	//// Check collision
	//	vec2 bottomLeft = vec2(m_position.x - PLAYER_SIZE / 2, m_position.y - PLAYER_SIZE / 2);
	//	vec2 topLeft = vec2(m_position.x - PLAYER_SIZE / 2, m_position.y + PLAYER_SIZE / 2);
	//	vec2 bottomRight = vec2(m_position.x + PLAYER_SIZE / 2, m_position.y - PLAYER_SIZE / 2);
	//	vec2 topRight = vec2(m_position.x + PLAYER_SIZE / 2, m_position.y + PLAYER_SIZE / 2);

	//	// Check bottom left point
	//	// Check if we collide with x displacement only
	//	vec2 blCheck = bottomLeft + vec2(displacement.x, 0.0f);
	//	vec2 blTile = m_pTilemap->GetTileAtWorldPos(blCheck);
	//	if (!(m_pTilemap->GetTileInfo((int)(m_pTilemap->GetInverseY(blTile.y)), (int)(blTile.x))->m_Walkable))
	//	{
	//		// Calculate overlap
	//		vec2 collisionTileTR(m_pTilemap->GetWorldPosAtTile(blTile + vec2(1.0f, 1.0f)));
	//		if (blCheck.x < collisionTileTR.x)
	//		{
	//			displacement.x -= (blCheck.x - collisionTileTR.x);
	//		}
	//	}

	//	// Check y displacement only
	//	blCheck = bottomLeft + vec2(0.0f, displacement.y);
	//	blTile = m_pTilemap->GetTileAtWorldPos(blCheck);
	//	if (!(m_pTilemap->GetTileInfo((int)(m_pTilemap->GetInverseY(blTile.y)), (int)(blTile.x))->m_Walkable))
	//	{
	//		// Calculate overlap
	//		vec2 collisionTileTR(m_pTilemap->GetWorldPosAtTile(blTile + vec2(1.0f, 1.0f)));
	//		if (blCheck.y < collisionTileTR.y)
	//		{
	//			displacement.y -= (blCheck.y - collisionTileTR.y);
	//		}
	//	}


	//	// Check bottom right point
	//	// Check if we collide with x displacement only
	//	vec2 brCheck = bottomRight + vec2(displacement.x, 0.0f);
	//	vec2 brTile = m_pTilemap->GetTileAtWorldPos(brCheck);
	//	if (!(m_pTilemap->GetTileInfo((int)(m_pTilemap->GetInverseY(brTile.y)), (int)(brTile.x))->m_Walkable))
	//	{
	//		// Calculate overlap
	//		vec2 collisionTileTL(m_pTilemap->GetWorldPosAtTile(brTile + vec2(0.0f, 1.0f)));
	//		if (brCheck.x > collisionTileTL.x)
	//		{
	//			displacement.x -= (brCheck.x - collisionTileTL.x);

	//			int x = 0;
	//		}
	//	}

	//	// Check y displacement only
	//	brCheck = bottomRight + vec2(-1.0f, displacement.y); // -1 needed for special case where bottom right is a multiple of 100.0f
	//	brTile = m_pTilemap->GetTileAtWorldPos(brCheck);
	//	if (!(m_pTilemap->GetTileInfo((int)(m_pTilemap->GetInverseY(brTile.y)), (int)(brTile.x))->m_Walkable))
	//	{
	//		// Calculate overlap
	//		vec2 collisionTileTL(m_pTilemap->GetWorldPosAtTile(brTile + vec2(0.0f, 1.0f)));
	//		if (brCheck.y < collisionTileTL.y)
	//		{
	//			displacement.y -= (brCheck.y - collisionTileTL.y);
	//		}
	//	}

	//	// Check top left point
	//	// Check if we collide with x displacement only
	//	vec2 tlCheck = topLeft + vec2(displacement.x, -1.0f);
	//	vec2 tlTile = m_pTilemap->GetTileAtWorldPos(tlCheck);
	//	if (!(m_pTilemap->GetTileInfo((int)(m_pTilemap->GetInverseY(tlTile.y)), (int)(tlTile.x))->m_Walkable))
	//	{
	//		// Calculate overlap
	//		vec2 collisionTileBR(m_pTilemap->GetWorldPosAtTile(tlTile + vec2(1.0f, 0.0f)));
	//		if (tlCheck.x < collisionTileBR.x)
	//		{
	//			displacement.x -= (tlCheck.x - collisionTileBR.x);
	//		}
	//	}

	//	// Check y displacement only
	//	tlCheck = topLeft + vec2(0.0f, displacement.y);
	//	tlTile = m_pTilemap->GetTileAtWorldPos(tlCheck);
	//	if (!(m_pTilemap->GetTileInfo((int)(m_pTilemap->GetInverseY(tlTile.y)), (int)(tlTile.x))->m_Walkable))
	//	{
	//		// Calculate overlap
	//		vec2 collisionTileBR(m_pTilemap->GetWorldPosAtTile(tlTile + vec2(1.0f, 0.0f)));
	//		if (tlCheck.y > collisionTileBR.y)
	//		{
	//			displacement.y -= (tlCheck.y - collisionTileBR.y);
	//		}
	//	}


	//	// Check top right point
	//	// Check if we collide with x displacement only
	//	vec2 trCheck = topRight + vec2(displacement.x, -1.0f);
	//	vec2 trTile = m_pTilemap->GetTileAtWorldPos(trCheck);
	//	if (!(m_pTilemap->GetTileInfo((int)(m_pTilemap->GetInverseY(trTile.y)), (int)(trTile.x))->m_Walkable))
	//	{
	//		// Calculate overlap
	//		vec2 collisionTileBL(m_pTilemap->GetWorldPosAtTile(trTile + vec2(0.0f, 0.0f)));
	//		if (trCheck.x > collisionTileBL.x)
	//		{
	//			displacement.x -= (trCheck.x - collisionTileBL.x);

	//			int x = 0;
	//		}
	//	}

	//	// Check y displacement only
	//	trCheck = topRight + vec2(-1.0f, displacement.y); // -1 needed for special case where bottom right is a multiple of 100.0f
	//	trTile = m_pTilemap->GetTileAtWorldPos(trCheck);
	//	if (!(m_pTilemap->GetTileInfo((int)(m_pTilemap->GetInverseY(trTile.y)), (int)(trTile.x))->m_Walkable))
	//	{
	//		// Calculate overlap
	//		vec2 collisionTileBL(m_pTilemap->GetWorldPosAtTile(trTile + vec2(0.0f, 0.0f)));
	//		if (trCheck.y > collisionTileBL.y)
	//		{
	//			displacement.y -= (trCheck.y - collisionTileBL.y);
	//		}
	//	}
	//}

	m_position += displacement;
}

PlayerController* Player::GetController()
{
    return m_pController;
}

void Player::SetGameSample(GameSample *gameSample)
{
	m_pGameSample = gameSample;
}

void Player::SetTilemap(Tilemap* tilemap)
{
	m_pTilemap = tilemap;
}