#ifndef __GameSample_H__
#define __GameSample_H__

class ShaderProgram;
class Player;
class SceneManager;
class MyResourceManager;
class EventManager;
class CollisionSystem;

#define TRIANGLE_SPEED      10.0f
#define CAMERA_SPEED        50.0f
#define CAMERA_ZOOM_SPEED   10.0f

#define WORLD_ORIGIN        vec2(50.0f, 50.0f)
#define WORLD_SIZE          vec2(150.0f, 150.0f)


namespace GameObjectType
{
    enum GameObjectType : unsigned int
    {
        Player,
        Camera,
        Target,
        Enemy,
        Tower,
		Bullet,
        Level,
        Misc
    };
}

class GameSample : public GameCore
{
public:
    GameSample(Framework* pFramework);
    virtual ~GameSample();

    virtual void OnSurfaceChanged(int width, int height) override;
    virtual void LoadContent() override;

    virtual void Update(float deltatime) override;
    virtual void Draw() override;

    virtual void AddEvent(Event* pEvent) override;
    bool ProcessEvent(Event* pEvent) override;

	ShaderProgram* GetShader(ShaderName::ShaderName shaderName) const;

	vec2 GetWorldOrigin();
	vec2 GetWorldSize();

    Player* GetPlayer();

protected:
    // Protected Member Variables
    std::map<ShaderName::ShaderName, ShaderProgram*> m_shaderMap;
    SceneManager* m_sceneManager;
    MyResourceManager* m_resourceManager;
    EventManager* m_eventManager;
    CollisionSystem* m_collisionSystem;

	Player *m_pPlayer;

	vec2 m_worldOrigin;
	vec2 m_worldSize;
};

#endif //__GameSample_H__