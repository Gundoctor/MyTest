#include "FrameworkPCH.h"


ScoreDisplay::ScoreDisplay(ShaderProgram* pShader, GLuint spriteStripID) :
	m_pShader(pShader),
	m_SpriteStripTexID(spriteStripID),
	m_VBO(0),
	m_IBO(0),
	m_Score(0)
{
	Initialize();
}

ScoreDisplay::~ScoreDisplay()
{
	// Release the VBO and IBO
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

/**
 *	Gen buffer and stuff?
 **/
void ScoreDisplay::Initialize()
{
	assert(m_VBO == 0);
	assert(m_IBO == 0);

	unsigned int neededNumOfVertices = NUM_SCORE_DIGIT * 4;
	ScoreVertexFormat *attributes = new ScoreVertexFormat[neededNumOfVertices];

	for (unsigned int i = 0; i < neededNumOfVertices; ++i)
	{
		float iNum = (float)(i / 4);
		float unitPos = NUM_SCORE_DIGIT - iNum;
		attributes[i].unitPosition = unitPos;
		attributes[i].u = (float)(i % 2);
		attributes[i].v = (float)(((i + 2) % 4) / 2);
		attributes[i].x = iNum + attributes[i].u;
		attributes[i].y = attributes[i].v * 2;
	}

	// Generate and fill buffer with our vertices
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ScoreVertexFormat) * neededNumOfVertices, attributes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Index buffer stuff
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	// Allocate memory for index buffer
	unsigned int *indices = new unsigned int[NUM_INDICES_NEEDED];
	for (unsigned int i = 0; i < NUM_SCORE_DIGIT; ++i)
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

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NUM_INDICES_NEEDED, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Cleanup memory allocated locally
	delete[] attributes;
	delete[] indices;

}

void ScoreDisplay::Draw(Vector2 scorePos, Vector2 camProj, Vector2 scoreScale)
{
	glUseProgram(m_pShader->GetProgram());
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	// -------------------ATTRIBUTES--------------------
	// Get the index of the position attribute in our shader and setup the attribute details.
	{
		GLint loc = glGetAttribLocation(m_pShader->GetProgram(), "a_UnitPos");
		if (loc != -1)
		{
			glVertexAttribPointer(loc, 1, GL_FLOAT, GL_FALSE, sizeof(ScoreVertexFormat), (void*)offsetof(ScoreVertexFormat, unitPosition));
			glEnableVertexAttribArray(loc);
		}

		loc = glGetAttribLocation(m_pShader->GetProgram(), "a_Position");
		if (loc != -1)
		{
			glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(ScoreVertexFormat), (void*)offsetof(ScoreVertexFormat, x));
			glEnableVertexAttribArray(loc);
		}

		loc = glGetAttribLocation(m_pShader->GetProgram(), "a_UVCoord");
		if (loc != -1)
		{
			glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(ScoreVertexFormat), (void*)offsetof(ScoreVertexFormat, u));
			glEnableVertexAttribArray(loc);
		}
	}


	// ---------------UNIFORMS------------------------
	{
		GLuint uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_WorldPosition");
		if (uLoc != -1)
		{
			glUniform2f(uLoc, scorePos.x, scorePos.y);
		}

		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_Scale");
		if (uLoc != -1)
		{
			glUniform2f(uLoc, scoreScale.x, scoreScale.y);
		}

		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_CamProjection");
		if (uLoc != -1)
		{
			glUniform2f(uLoc, camProj.x, camProj.y);
		}

		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_Score");
		if (uLoc != -1)
		{
			glUniform1i(uLoc, m_Score);
		}
	}


	// ---------------TEXTURE------------------------
	{
		glBindTexture(GL_TEXTURE_2D, m_SpriteStripTexID);
		GLuint tLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_Texture");
		if (tLoc != -1)
		{
			glUniform1i(tLoc, 0);
		}
	}


	// DRAW STUFF
	glDrawElements(GL_TRIANGLES, NUM_INDICES_NEEDED, GL_UNSIGNED_INT, (void*)0);

	CheckForGLErrors();
}

void ScoreDisplay::UpdateScore(int score)
{
	m_Score = score;
}

int ScoreDisplay::GetScore()
{
	return m_Score;
}

