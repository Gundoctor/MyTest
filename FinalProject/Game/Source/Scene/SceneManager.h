#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

class GameSample;
class MyResourceManager;
class CollisionSystem;
class Player;

namespace SceneName
{
	enum SceneName : unsigned int
	{
		Game1,
        Tilemap,
		Win
	};
}


class SceneManager : public SceneManagerBase
{
public:
    SceneManager(GameSample* game, MyResourceManager* meshManager, CollisionSystem* collisionSystem);
    ~SceneManager();

	// To be called after MeshManager has been set up
	void LoadContent() override;
    bool OnEvent(Event *pEvent);

    bool SetCurrentScene(SceneName::SceneName scene);
	ShaderProgram* GetShaderFromGame(ShaderName::ShaderName sName) const;

    GameSample* GetGame();

    void SetPlayer(Player *player);
    Player* GetPlayer();


private:
    // Member variables
    GameSample *m_game;
    MyResourceManager* m_resourceManager;
    CollisionSystem* m_collisionSystem;

	Player *m_pPlayer;
};

#endif