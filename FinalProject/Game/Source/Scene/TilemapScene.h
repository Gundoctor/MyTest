#ifndef __TILEMAPSCENE_H__
#define __TILEMAPSCENE_H__

class ShaderProgram;
class Tilemap;
class ScoreDisplay;
class Pathfinder;
class Mesh;

#define PLAYER_COLOR { 255, 0, 0, 255 }
#define SCORE_CAM_PROJ vec2(200.0f, 200.0f)
#define ENEMY_POOL_SIZE 50
#define BULLET_POOL_SIZE 600
#define TOWER_POOL_SIZE  50
#define VALID_SPAWN_TILE_COL {0, 255, 0, 150}
#define INVALID_SPAWN_TILE_COL {255, 0, 0, 150}

#define TOWER_COST			150
#define ENEMY_SPAWN_TIME	1.5f // 1 enemy per 2.5 seconds
#define MOUSE_TIME			0.1f // Wait at least 0.1 seconds before calculating pathfinding

#define INITIAL_SCORE 500
#define BULLET_DAMAGE 30

#define SAVE_FILE_LOC "Data/SaveFiles/TilemapSceneSave.json"

class TilemapScene : public Scene
{
private:
	SceneManager *			m_sceneManager;
	MyResourceManager*      m_resourceManager;
	CollisionSystem*        m_collisionSystem;

	ParticleSystem* m_particleSystem;
	ParticleRenderer* m_particleRenderer;
	ParticleEmitter* m_pEmitter = nullptr;
	ScoreDisplay* m_scoreDisp;

	std::vector<Tilemap*> m_pTilemaps;
    Tilemap* m_pCurrTilemap;
	Tilemap* m_pSmallTilemap;
	Tilemap* m_pMediumTilemap;
	Tilemap* m_pLargeTilemap;
	int m_currTilemapIndex;

	Pathfinder *m_pPathfinder;
	vec2 m_EndZone;
	vec2 m_StartZone;

	StaticCamera* m_pStaticCam;
	StaticCamera* m_pScoreCam;

	float m_enemySpawnTimer;
	std::vector<Enemy*> m_pEnemies;
	std::vector<Bullet*> m_pBullets;
	std::vector<Tower*> m_pTowers;

	SpriteFormat m_towerSpriteInfo;

	// Mouse Overlay
	Mesh* m_mouseMesh;
	ShaderProgram* m_mouseShader;
	vec2 m_mouseMeshPos;
	vec2 m_mouseTilePos;
	bool m_mouseIsOnValidLocation;
	float m_mouseTimer;

	void LoadResources();

	void ModifyScore(int amount);
	
	bool GetSpriteInfo(SpriteFormat& sprite, std::string filename, std::string jsonName);
	bool GetMultipleSpriteInfo(std::vector<SpriteFormat>& spriteVec, std::string filename, std::vector<std::string> stringsToFind);
	void FindJSONSpriteInfo(SpriteFormat& sprite, cJSON* jObject);
	vec2 GetSpriteSize(std::string filename);

	vec2 CalculateMouseInWorldSpace(float x, float y);


	// Initialization functions
	void InitializeEnemies();
	void InitializeBullets();
	void InitializeTowers();
	void DeactivateAllEnemies();
	void DeactivateAllBullets();
	void DeactivateAllTowers();

	void SpawnEnemy();
	void SpawnTowerAtLocation(vec2 pos);

	void UpdateMouseOverlay(float deltatime);
	void ChangeActiveTilemap(Tilemap* tilemap);

	int SaveGame();
	int LoadGame();

public:
	TilemapScene(SceneManager *sceneManager, MyResourceManager *resourceManager, CollisionSystem* collisionSystem);
    virtual ~TilemapScene();

    virtual void OnSurfaceChanged(int width, int height);
    virtual void LoadContent() override;
    virtual void UpdateCameraViewports(const GameCore* game) override;

    virtual void Update(float deltatime) override;
    virtual void Draw() override;

	virtual bool OnEvent(Event *pEvent) override;

	// Get EndZone location
	vec2 GetEndZone();
	Tilemap* GetCurrentTilemap();
	Pathfinder* GetPathfinder();

	// Get inactive game objects
	Enemy* FindClosestEnemy(Tower* pTower);
	Enemy* GetInactiveEnemy();
	Bullet* GetInactiveBullet();
	Tower* GetInactiveTower();
};

#endif //__TILEMAPSCENE_H__