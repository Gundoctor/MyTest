#include "FrameworkPCH.h"

SceneManagerBase::SceneManagerBase() :
    m_currentScene(nullptr)
{

}

SceneManagerBase::~SceneManagerBase()
{
}

void SceneManagerBase::Update(float deltatime)
{
    assert(m_currentScene != nullptr);

    m_currentScene->Update(deltatime);
}

void SceneManagerBase::Draw()
{
    assert(m_currentScene != nullptr);

    m_currentScene->Draw();
}

void SceneManagerBase::UpdateCameraViewports(const GameCore* game)
{
    if (m_currentScene != nullptr)
    {
        m_currentScene->UpdateCameraViewports(game);
    }
}

bool SceneManagerBase::SetCurrentScene(unsigned int sceneName)
{
    std::map<unsigned int, Scene*>::iterator foundScene;
    if ((foundScene = m_scenes.find(sceneName)) != m_scenes.end())
    {
        m_currentScene = foundScene->second;
        return true;
    }

    return false;
}