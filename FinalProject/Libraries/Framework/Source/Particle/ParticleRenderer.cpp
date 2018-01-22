#include "FrameworkPCH.h"

ParticleRenderer::ParticleRenderer(ShaderProgram* pShader, GLuint texID, SpriteFormat sprite) :
	m_pShader(pShader),
	m_VBO(0),
	m_IBO(0), 
	m_TexID(texID),
	m_spriteInfo(sprite),
	m_MaxIndices(6 * MAX_NUM_PARTICLE),
	m_MaxVertices(4 * MAX_NUM_PARTICLE),
	m_NumActiveParticles(0),
	m_pVertexArray(nullptr),
	m_spritesheetSize(1.0f, 1.0f)
{
	Initialize();
}

ParticleRenderer::~ParticleRenderer()
{
	Cleanup();
}


void ParticleRenderer::Initialize()
{
	assert(m_VBO == 0);
	assert(m_IBO == 0);
	assert(m_pVertexArray == nullptr);


	// Generate and fill buffer with our vertices
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Allocate memory for vertex buffer
	m_pVertexArray = new ParticleAttribute[m_MaxVertices];
	for (unsigned int i = 0; i < m_MaxVertices; ++i)
	{
		m_pVertexArray[i].Position.Set(0.0f, 0.0f);
		m_pVertexArray[i].u = (float)(i % 2);
		m_pVertexArray[i].v = (float)(((i + 2) % 4) / 2);

		m_pVertexArray[i].r = 255;
		m_pVertexArray[i].g = 255;
		m_pVertexArray[i].b = 255;
		m_pVertexArray[i].a = 255;
	}

	// Generate and fill index buffer
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	// Allocate memory for index buffer
	unsigned int *indices = new unsigned int[m_MaxIndices];
	for (unsigned int i = 0; i < MAX_NUM_PARTICLE; ++i)
	{
		for (unsigned int j = 0; j < 6; ++j)
		{
			// assumes vertices follow a pattern of
			// tl, tr, bl, br
			//  0,  1,  2,  3
			unsigned int vertexIndex = j % 3 + j / 3;
			indices[i * 6 + j] = i * 4 + vertexIndex;
		}
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_MaxIndices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Cleanup memory allocated locally
	delete[] indices;
}

void ParticleRenderer::Cleanup()
{
	// Release the VBO and IBO
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);

	if (m_pVertexArray != nullptr)
		delete[] m_pVertexArray;
}

void ParticleRenderer::AddParticle(Vector2 position, Vector2 scale)
{
	// Add 4 points to the next particle
	unsigned int particleIndexOffset = m_NumActiveParticles * 4;
	m_pVertexArray[particleIndexOffset + 0].Position.Set(-1.0f, 1.0f); // top left
	m_pVertexArray[particleIndexOffset + 1].Position.Set(1.0f, 1.0f); // top right
	m_pVertexArray[particleIndexOffset + 2].Position.Set(-1.0f, -1.0f); // bottom left
	m_pVertexArray[particleIndexOffset + 3].Position.Set(1.0f, -1.0f); // bottom right
	for (unsigned int i = 0; i < 4; ++i)
	{
		m_pVertexArray[particleIndexOffset + i].Position *= scale;
		m_pVertexArray[particleIndexOffset + i].Position += position;
	}

	// Add to the number of active particles
	++m_NumActiveParticles;
}

void ParticleRenderer::Draw(Vector2 cameraPos, Vector2 cameraProj)
{
	assert(m_pShader != nullptr);
	glUseProgram(m_pShader->GetProgram());

	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Copy local heap memory to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleAttribute) * 4 * m_NumActiveParticles, m_pVertexArray, GL_DYNAMIC_DRAW);

	// Set up shader code
	// ATTRIBUTES
	{
		GLint loc = glGetAttribLocation(m_pShader->GetProgram(), "a_Position");
		if (loc != -1)
		{
			glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleAttribute), (void*)0);
			glEnableVertexAttribArray(loc);
		}

		loc = glGetAttribLocation(m_pShader->GetProgram(), "a_UVCoord");
		if (loc != -1)
		{
			glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleAttribute), (void*)offsetof(ParticleAttribute, u));
			glEnableVertexAttribArray(loc);
		}

		loc = glGetAttribLocation(m_pShader->GetProgram(), "a_Color");
		if (loc != -1)
		{
			glVertexAttribPointer(loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ParticleAttribute), (void*)offsetof(ParticleAttribute, r));
			glEnableVertexAttribArray(loc);
		}
	}

	// UNIFORMS
	{
		GLint uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_CameraPos");
		if (uLoc != -1)
		{
			glUniform2f(uLoc, cameraPos.x, cameraPos.y);
		}

		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_ProjectionScale");
		if (uLoc != -1)
		{
			glUniform2f(uLoc, cameraProj.x, cameraProj.y);
		}

		glBindTexture(GL_TEXTURE_2D, m_TexID);
		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_Texture");
		if (uLoc != -1)
		{
			glUniform1i(uLoc, 0);
		}

		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_uvOffset");
		if (uLoc != -1)
		{
			glUniform2f(uLoc, m_spriteInfo.uvOffset.x, m_spriteInfo.uvOffset.y);
		}

		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_uvSize");
		if (uLoc != -1)
		{
			glUniform2f(uLoc, m_spriteInfo.uvSize.x, m_spriteInfo.uvSize.y);
		}

		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_spritesheetSize");
		if (uLoc != -1)
		{
			glUniform2f(uLoc, m_spritesheetSize.x, m_spritesheetSize.y);
		}

	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glDrawElements(GL_TRIANGLES, 6 * m_NumActiveParticles, GL_UNSIGNED_INT, (void*)0);

	// Check for errors.
	CheckForGLErrors();
}

void ParticleRenderer::ResetActiveParticleCount()
{
	m_NumActiveParticles = 0;
}

void ParticleRenderer::SetSpritesheetSize(vec2 size)
{
	m_spritesheetSize = size;
}