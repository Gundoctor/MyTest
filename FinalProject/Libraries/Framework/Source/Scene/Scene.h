#ifndef __SCENE_H__
#define __SCENE_H__

class Event;
class GameObject;
class ShaderProgram;
class SceneNode;

class Scene
{
public:
    Scene();
    virtual ~Scene();

    virtual void Update(float deltatime) = 0;
    virtual void Draw() = 0;

	virtual void LoadContent() = 0;
    virtual void UpdateCameraViewports(const GameCore* game);
    
    virtual bool OnEvent(Event *pEvent);


protected:
	SceneNode* m_pRoot;
};

#endif