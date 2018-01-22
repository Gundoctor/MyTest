#ifndef __BULLET_H__
#define __BULLET_H__

#define BULLET_SPEED 600.0f
#define BULLET_LIFE  2.0f // 2 seconds

class ShaderProgram;
class SceneNode;
class Mesh;

class Bullet : public GameObject
{
public:
	Bullet(vec2 pos, vec2 scale, Mesh* pMesh);
	Bullet(SceneNode* parent, vec2 pos, vec2 scale, Mesh* pMesh);
	~Bullet();


	void SetVelocity(vec2 dirVec);

	virtual void Deactivate() override;

protected:
	virtual void UpdateCurrent(float deltatime);
	
private:
	vec2 m_velocity;
	float m_lifespan;
};

#endif