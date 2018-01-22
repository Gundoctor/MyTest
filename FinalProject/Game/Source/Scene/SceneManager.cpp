#include "GamePCH.h"


SceneManager::SceneManager(GameSample *game, MyResourceManager *meshManager, CollisionSystem* collisionSystem) : 
    m_game(game),
    m_resourceManager(meshManager),
    m_collisionSystem(collisionSystem)
{
}

SceneManager::~SceneManager()
{
    for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
    {
        delete it->second;
    }
    m_scenes.clear();
}

void SceneManager::LoadContent()
{
	// Instantiate and Initialize Scenes
    // Scene *game1Scene = new Game1Scene(this, m_resourceManager, m_collisionSystem);
    Scene *tilemapScene = new TilemapScene(this, m_resourceManager, m_collisionSystem);

    // Register Scenes to the map
    // m_scenes.insert(std::pair<SceneName::SceneName, Scene*>(SceneName::Game1, game1Scene));
	m_scenes.insert(std::pair<SceneName::SceneName, Scene*>(SceneName::Tilemap, tilemapScene));


	// Load up all Scenes
	for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
	{
        // Load/Generate Scene
		it->second->LoadContent();
	}
}

bool SceneManager::SetCurrentScene(SceneName::SceneName sceneName)
{
    return SceneManagerBase::SetCurrentScene(sceneName);
}

bool SceneManager::OnEvent(Event *pEvent)
{
    if (m_currentScene != nullptr)
    {
        return m_currentScene->OnEvent(pEvent);
    }
	return false;
}

ShaderProgram* SceneManager::GetShaderFromGame(ShaderName::ShaderName sName) const
{
	return m_game->GetShader(sName);
}

void SceneManager::SetPlayer(Player *player)
{
	m_pPlayer = player;
}

Player* SceneManager::GetPlayer()
{
    return m_pPlayer;
}

GameSample* SceneManager::GetGame()
{
    return m_game;
}