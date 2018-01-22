#include "GamePCH.h"

Tilemap::Tilemap(Mesh* tileMesh, vec2 pos, vec2 scale) :
	SceneNode(pos, scale),
	m_Tiles(nullptr),
	m_TilesBackup(nullptr)
{
    m_Tile = tileMesh;
}

Tilemap::Tilemap(Mesh *tileMesh, SceneNode* pParent, vec2 pos, vec2 scale) :
	SceneNode(pParent, pos, scale),
	m_Tiles(nullptr),
	m_TilesBackup(nullptr)
{
	m_Tile = tileMesh;
}

Tilemap::~Tilemap()
{
	delete[] m_Tiles;
	delete[] m_TilesBackup;
}

void Tilemap::ReloadTilemap()
{
	for (int i = 0; i < m_mapSize.x * m_mapSize.y; ++i)
	{
		m_Tiles[i] = m_TilesBackup[i];
	}
}

void Tilemap::InitializeTilemapSmall()
{
	// Grass 0
	// Water 1
	// Wall  2
	// Start 3
	// End   4
	m_TileInfo[TileType::Grass].m_Walkable = true;
	m_TileInfo[TileType::Water].m_Walkable = false;
	m_TileInfo[TileType::Wall].m_Walkable = false;
	m_TileInfo[TileType::Start].m_Walkable = true;
	m_TileInfo[TileType::End].m_Walkable = true;
	m_TileInfo[TileType::UNWALKABLE].m_Walkable = false;

	m_mapSize.Set(MAP_SMALL_WIDTH, MAP_SMALL_HEIGHT);
	m_worldSize.Set((float)(m_mapSize.x * TILE_SIZE), (float)(m_mapSize.y * TILE_SIZE));

	unsigned char tiles[] = {
		2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 1, 0, 0, 0, 0, 0, 4, 2,
		2, 0, 0, 0, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 0, 2,
		2, 3, 0, 0, 0, 1, 1, 0, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2,
	};

	SetTileArray(tiles, m_mapSize.y, m_mapSize.x);

	m_startZone.Set(1.0f, 1.0f);
	m_endZone.Set(MAP_SMALL_WIDTH - 2.0f, MAP_SMALL_HEIGHT - 2.0f);
}

void Tilemap::InitializeTilemapMedium()
{
    // Grass 0
    // Water 1
    // Wall  2
	// Start 3
	// End   4
    m_TileInfo[TileType::Grass].m_Walkable = true;
    m_TileInfo[TileType::Water].m_Walkable = false;
    m_TileInfo[TileType::Wall].m_Walkable = false;
	m_TileInfo[TileType::Start].m_Walkable = true;
	m_TileInfo[TileType::End].m_Walkable = true;
	m_TileInfo[TileType::UNWALKABLE].m_Walkable = false;

	m_mapSize.Set(MAP_MEDIUM_WIDTH, MAP_MEDIUM_HEIGHT);
	m_worldSize.Set((float)(m_mapSize.x * TILE_SIZE), (float)(m_mapSize.y * TILE_SIZE));

    unsigned char tiles[] = { 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 1, 4, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 1, 1, 1, 0, 0, 1, 1, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2,
        2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
        2, 0, 0, 0, 0, 1, 0, 1, 1, 1, 2,
        2, 3, 0, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    };
    
	SetTileArray(tiles, m_mapSize.y, m_mapSize.x);

	m_startZone.Set(1.0f, 1.0f);
	m_endZone.Set(MAP_MEDIUM_WIDTH - 2.0f, MAP_MEDIUM_HEIGHT - 2.0f);
}

void Tilemap::InitializeTilemapLarge()
{
	// Grass 0
	// Water 1
	// Wall  2
	// Start 3
	// End   4
	m_TileInfo[TileType::Grass].m_Walkable = true;
	m_TileInfo[TileType::Water].m_Walkable = false;
	m_TileInfo[TileType::Wall].m_Walkable = false;
	m_TileInfo[TileType::Start].m_Walkable = true;
	m_TileInfo[TileType::End].m_Walkable = true;
	m_TileInfo[TileType::UNWALKABLE].m_Walkable = false;

	m_mapSize.Set(MAP_LARGE_WIDTH, MAP_LARGE_HEIGHT);
	m_worldSize.Set((float)(m_mapSize.x * TILE_SIZE), (float)(m_mapSize.y * TILE_SIZE));

	unsigned char tiles[] = {
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 4, 2,
		2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2,
		2, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2,
		2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 2,
		2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
		2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
		2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 2,
		2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 2,
		2, 3, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	};

	SetTileArray(tiles, m_mapSize.y, m_mapSize.x);

	m_startZone.Set(1.0f, 1.0f);
	m_endZone.Set(MAP_LARGE_WIDTH - 2.0f, MAP_LARGE_HEIGHT - 2.0f);
}

void Tilemap::SetTileArray(unsigned char tiles[], unsigned int numRows, unsigned int numCols)
{
	// This is so that the initialize functions can only be called once
	assert(m_Tiles == nullptr);

	m_Tiles = new unsigned char[numCols * numRows];
	m_TilesBackup = new unsigned char[numCols * numRows];
	for (unsigned int i = 0; i < numCols *numRows; ++i)
	{
		m_Tiles[i] = tiles[i];
		m_TilesBackup[i] = tiles[i];
	}
}

void Tilemap::DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 camProj, ShaderProgram* camShader)
{
    for (unsigned int row = 0; row < (unsigned int)m_mapSize.y; ++row)
    {
        for (unsigned int col = 0; col < (unsigned int)m_mapSize.x; ++col)
        {
            vec2 tilePos((float)col * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE * m_mapSize.y - (float)(row + 1) * TILE_SIZE + TILE_SIZE / 2);
			tilePos += worldPos;

            unsigned char tileIndex = m_Tiles[GetTileIndex(row, col)];

			if (m_TileInfo[tileIndex].m_Texture == 0)
				continue;
    
            if (camShader == nullptr)
            {
                m_Tile->DrawSprite(vec2(TILE_SIZE / 2, TILE_SIZE / 2), tilePos, camPos, camProj, m_pShader, m_TileInfo[tileIndex].m_Texture, m_TileInfo[tileIndex].m_SpriteInfo);
            }
            else
            {
                m_Tile->DrawSprite(vec2(TILE_SIZE / 2, TILE_SIZE / 2), tilePos, camPos, camProj, camShader, m_TileInfo[tileIndex].m_Texture, m_TileInfo[tileIndex].m_SpriteInfo);
            }
            
        }
    }
}

void Tilemap::DrawWalkabilityMap()
{
	for (unsigned int row = 0; row < (unsigned int)m_mapSize.y; ++row)
	{
		for (unsigned int col = 0; col < (unsigned int)m_mapSize.x; ++col)
		{
			unsigned int index = GetTileIndex(row, col);

			// OutputMessage("%.1f ", m_pNodes[index].GetGScore());
			if (m_TileInfo[m_Tiles[index]].m_Walkable)
			{
				OutputMessage("%d\t\t", index);
			}
			else
			{
				OutputMessage("X\t\t");
			}
		}
		OutputMessage("\n");
	}
}

void Tilemap::SetTileType(int row, int col, unsigned char tile)
{
	if (row * col > m_mapSize.x * m_mapSize.y || row * col < 0)
		return;

	m_Tiles[GetTileIndex(row, col)] = tile;
}

unsigned char Tilemap::GetTileType(int row, int col)
{
	if (row * col > m_mapSize.x * m_mapSize.y || row * col < 0)
		return 0;

	return m_Tiles[GetTileIndex(row, col)];
}

TileInfo* Tilemap::GetTileInfo(int row, int col)
{
    if (row * col > m_mapSize.x * m_mapSize.y || row * col < 0)
        return 0;

    return &(m_TileInfo[m_Tiles[GetTileIndex(row, col)]]);
}

TileInfo* Tilemap::GetTileInfo(int index)
{
	if (index > m_mapSize.x * m_mapSize.y || index < 0)
		return 0;

	return &(m_TileInfo[m_Tiles[index]]);
}

vec2 Tilemap::GetWorldSize()
{
	return m_worldSize;
}

Vector2Int Tilemap::GetMapSize()
{
	return m_mapSize;
}

bool Tilemap::IsValidPos(int row, int col)
{
	// y
	if (row < 0 || row > m_mapSize.y - 1)
		return false;

	// x
	if (col < 0 || col > m_mapSize.x - 1)
		return false;

	return true;
}

float Tilemap::GetCostForTile(int row, int col)
{
	unsigned char tileType = m_Tiles[GetTileIndex(row, col)];
	float cost = -1.0f;
	switch (tileType)
	{
	case TileType::Grass:
		cost = 1.0f;
		break;
	case TileType::Wall:
		cost = 10.0f;
		break;
	case TileType::Water:
		cost = 5.0f;
		break;
	default:
		cost = 1.0f;
		break;
	}

	return cost;
}

unsigned int Tilemap::GetTileIndex(int row, int col)
{
    return row * m_mapSize.x + col;
}

void Tilemap::SetTileMesh(TileType::TileType index, GLuint texture, SpriteFormat sprite)
{
    m_TileInfo[index].m_Texture = texture;
	m_TileInfo[index].m_SpriteInfo.uvOffset = sprite.uvOffset;
	m_TileInfo[index].m_SpriteInfo.uvSize = sprite.uvSize;
}

vec2 Tilemap::GetTileAtWorldPos(vec2 worldPosition)
{
	int tileX = (int)(worldPosition.x / TILE_SIZE);
	int tileY = (int)(worldPosition.y / TILE_SIZE);

	if (worldPosition.x < 0)
		tileX -= 1;
	if (worldPosition.y < 0)
		tileY -= 1;

	return vec2((float)tileX, (float)tileY);
}

vec2 Tilemap::GetWorldPosAtTile(vec2 tilePos)
{
	float worldX = tilePos.x * TILE_SIZE + TILE_SIZE / 2.0f;
	float worldY = tilePos.y * TILE_SIZE + TILE_SIZE / 2.0f;
	return vec2(worldX, worldY);
}

float Tilemap::GetInverseY(float y)
{
	return m_mapSize.y - y - 1;
}

ShaderProgram* Tilemap::GetShader()
{
    return m_pShader;
}

void Tilemap::SetShader(ShaderProgram* pShader)
{
    m_pShader = pShader;
}

void Tilemap::UpdateWalkability(int row, int col, bool walkable)
{
	if (IsValidPos(row, col))
	{
		int tileIndex = row * m_mapSize.x + col;
		m_Tiles[tileIndex] = TileType::UNWALKABLE;
	}
}

vec2 Tilemap::GetStartPos()
{
	return m_startZone;
}

vec2 Tilemap::GetEndPos()
{
	return m_endZone;
}