#ifndef __SCENEMANAGERBASE_H__
#define __SCENEMANAGERBASE_H__

class Scene;

class SceneManagerBase
{
public:
    SceneManagerBase();
    virtual ~SceneManagerBase();

    // To be called after MeshManager has been set up
    virtual void LoadContent() = 0;
    virtual void Update(float deltatime);
    virtual void Draw();

    virtual void UpdateCameraViewports(const GameCore* game);
    virtual bool OnEvent(Event *pEvent) { return false; }

protected:
    bool SetCurrentScene(unsigned int scene);
    
    std::map<unsigned int, Scene*> m_scenes;
    Scene *m_currentScene;

};

#endif