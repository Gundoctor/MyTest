#ifndef __TILEMAP_H__
#define __TILEMAP_H__

class ShaderProgram;
class Mesh;
class MyResourceManager;

struct TileInfo
{
    GLuint              m_Texture = 0;
	SpriteFormat		m_SpriteInfo;
    bool                m_Walkable;
};


#define TILE_SIZE   100


#define MAP_SMALL_WIDTH    9
#define MAP_SMALL_HEIGHT   7
#define MAP_MEDIUM_WIDTH   11
#define MAP_MEDIUM_HEIGHT  11
#define MAP_LARGE_WIDTH    24
#define MAP_LARGE_HEIGHT   16


class Tilemap : public SceneNode
{
public:
    Tilemap(Mesh *tileMesh, vec2 pos, vec2 scale);
	Tilemap(Mesh *tileMesh, SceneNode* pParent, vec2 pos, vec2 scale);
    ~Tilemap();

    void InitializeTilemapSmall();
	void InitializeTilemapMedium();
	void InitializeTilemapLarge();

	void DrawWalkabilityMap();

	/////////// GETTERS
	void ReloadTilemap();

	unsigned char GetTileType(int row, int col);
    TileInfo* GetTileInfo(int row, int col);
	TileInfo* GetTileInfo(int index);
	vec2 GetWorldSize();
	Vector2Int GetMapSize();

	bool IsValidPos(int row, int col);

	float GetCostForTile(int row, int col);
	vec2 GetTileAtWorldPos(vec2 worldPosition);
	vec2 GetWorldPosAtTile(vec2 tilePos);
	float GetInverseY(float y);

    ShaderProgram* GetShader();

	vec2 GetStartPos();
	vec2 GetEndPos();

	////////// SETTERS
	void SetTileType(int row, int col, unsigned char tile);
	void SetTileMesh(TileType::TileType index, GLuint texture, SpriteFormat sprite);
    void SetShader(ShaderProgram* pShader);

	void UpdateWalkability(int row, int col, bool walkable);

protected:
	void DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 camProj, ShaderProgram* camShader);

private:
    Mesh* m_Tile;
    ShaderProgram* m_pShader;

    unsigned char*      m_Tiles;
	unsigned char*      m_TilesBackup;

    TileInfo            m_TileInfo[TileType::MAX_NUM];

	vec2				m_worldSize;
	Vector2Int			m_mapSize;

	vec2				m_startZone;
	vec2				m_endZone;

    // Private Methods
    unsigned int GetTileIndex(int row, int col);
	void SetTileArray(unsigned char tiles[], unsigned int numRows, unsigned int numCols);
};



#endif
