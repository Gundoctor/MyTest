#ifndef __PARTICLERENDERER_H__
#define __PARTICLERENDERER_H__


class ShaderProgram;

class ParticleRenderer
{
	struct ParticleAttribute
	{
		Vector2 Position;
		unsigned char r, g, b, a;
		float u, v;
	};

public:
	ParticleRenderer(ShaderProgram* pShader, GLuint texID, SpriteFormat sprite);
	~ParticleRenderer();

	void AddParticle(Vector2 position, Vector2 scale);
	void Draw(Vector2 cameraPos, Vector2 cameraProj);
	void ResetActiveParticleCount();

	void SetSpritesheetSize(vec2 size);

private:
	// MEMBER VARIABLES
	ShaderProgram * m_pShader;
	GLuint m_VBO;
	GLuint m_IBO;
	GLuint m_TexID;

	SpriteFormat m_spriteInfo;

	unsigned int m_MaxIndices;
	unsigned int m_MaxVertices;

	unsigned int m_NumActiveParticles;
	ParticleAttribute *m_pVertexArray;

	vec2 m_spritesheetSize;


	// PRIVATE METHODS
	void Initialize();
	void Cleanup();


};

#endif