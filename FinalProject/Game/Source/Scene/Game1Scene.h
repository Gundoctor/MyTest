#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#define ZONE_BUFFER         6.0f
#define START_ZONE          vec2(WORLD_ORIGIN.x - (WORLD_SIZE.x / 2) + ZONE_BUFFER, WORLD_ORIGIN.y + (WORLD_SIZE.y / 2) - ZONE_BUFFER)
#define END_ZONE            vec2(WORLD_ORIGIN.x + (WORLD_SIZE.x / 2) - ZONE_BUFFER, WORLD_ORIGIN.y - (WORLD_SIZE.y / 2) + ZONE_BUFFER)
#define MINIMAP_CAM_PROJECTION_BUFFER   vec2(10.0f, 10.0f)
#define ENEMYPOOL_SIZE					40
#define ENEMY_SCALE						vec2(5.0f, 5.0f)
#define TRANSITION_SPEED                vec2(70.0f, 70.0f)        // 0.8, 0.8 per second
#define MAX_TRANSITION_SCALE            80.0f
#define PROJECTION_SCALE                vec2(40.0f, 40.0f)

#define SAVEFILE_LOC        "Data/SaveFiles/GameplaySceneSave.json"

class MyResourceManager;
class SceneManager;
class CollisionSystem;

class Game1Scene : public Scene
{
public:
	Game1Scene(SceneManager *sceneManager, MyResourceManager *meshManager, CollisionSystem* collisionSystem);
	virtual ~Game1Scene();

	void Update(float deltatime) override;
	void Draw() override;

	void LoadContent() override;
	bool OnEvent(Event *pEvent) override;
    void UpdateCameraViewports(const GameCore* game) override;

    void ReloadActiveGameObjects();
    void ReloadLevel(unsigned int numberOfEnemies);

private:
	vec2 GetValidRandomPosition();
    void StartTransition();
    void EndTransition();
    void ReloadSameLevel();

    int SaveScene();
    int LoadScene();

private:
    SceneManager*           m_sceneManager;
    MyResourceManager*      m_resourceManager;
    CollisionSystem*        m_collisionSystem;

    std::vector<Camera*>       m_cameras;
    std::vector<GameObject*>   m_localGameObjects;

	std::vector<SimpleObject*>		m_enemyPool;
	SimpleObject*					m_target;

    ControllableCamera*     m_pControlCam;
    SimpleObject*           m_level;
    SimpleObject*           m_starTransitionObject;


    unsigned int            m_enemyCount;
    bool                    m_transitionIsOn;
    bool                    m_controlIsOn;

};

#endif