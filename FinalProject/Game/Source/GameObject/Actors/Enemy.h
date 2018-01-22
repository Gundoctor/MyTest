#ifndef __ENEMY_H__
#define __ENEMY_H__

#define ENEMY_SPEED 100.0f // 1 tile per second
#define ENEMY_SPRITE_TIMER (1.0f / 2.0f)	// 2 frames per second

class ShaderProgram;
class Mesh;
class Pathfinder;
class TilemapScene;

namespace EnemyDir
{
	enum EnemyDir
	{
		Left,
		Right,
		Up,
		Down
	};
}

class Enemy : public GameObject
{
public:
    Enemy(vec2 pos, vec2 scale, Mesh* pMesh, GLuint texID, TilemapScene* scene);
    Enemy(SceneNode* parent, vec2 pos, vec2 scale, Mesh* pMesh, GLuint texID, TilemapScene* scene);
    ~Enemy();

	virtual void Activate() override;
	void SetDirection(vec2 dir);
	void SetSpeed(float speed);
	void InsertSpriteFormat(EnemyDir::EnemyDir dir, SpriteFormat sprite);

protected:
    // Protected Methods
    void UpdateCurrent(float deltatime) override;
    void DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 projScale, ShaderProgram* pShader = 0) override;

private:
	TilemapScene * m_scene;

	GLuint m_TexID;

	std::vector<SpriteFormat> m_spriteLeft;
	std::vector<SpriteFormat> m_spriteRight;
	std::vector<SpriteFormat> m_spriteUp;
	std::vector<SpriteFormat> m_spriteDown;
	SpriteFormat m_currSprite;
	unsigned int m_currSpriteIndex;

	float m_speed;
	vec2 m_direction;

	vec2 m_target;

	float m_spriteTimer;

	bool HasReachedTarget();
	bool UpdateTargetWithNewNode();
};

#endif