#ifndef __TOWER_H__
#define __TOWER_H__

#define RELOAD_TIME 1.0f
#define SHOT_COUNT  6
#define TOWER_RANGE 300.0f
#define RATE_OF_FIRE 2.0f // 2 shots per second

class ShaderProgram;
class Mesh;
class TilemapScene;

class Tower : public GameObject
{
    typedef void (Tower::*AIStateMethod)(float deltatime);

public:
    Tower(TilemapScene* pScene, vec2 pos, vec2 scale, Mesh* pMesh, GLuint texId, SpriteFormat sprite);
    Tower(TilemapScene* pScene, SceneNode* parent, vec2 pos, vec2 scale, Mesh* pMesh, GLuint texId, SpriteFormat sprite);
    ~Tower();

	virtual float GetSize() const override;

protected:
    void UpdateCurrent(float deltatime) override;
	void DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 camProj, ShaderProgram* camShader) override;

private:
	TilemapScene* m_pTilemapScene;
	float m_timeToShoot;
	float m_reloadTimer;
	int m_shotsLeft;

	GLuint m_TexID;
	SpriteFormat m_spriteInfo;

	Enemy* m_pTarget;

    AIStateMethod m_pAIStateMethod;
	AIStateMethod m_pPrevAIStateMethod;

	void ChangeAIState(AIStateMethod pMethod);
    void AIState_Idle(float deltatime);
    void AIState_Reload(float deltatime);
    void AIState_Attack(float deltatime);

};

#endif