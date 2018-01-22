#include "FrameworkPCH.h"

Mesh::Mesh() : 
    m_numOfVertices(0),
    m_numOfIndices(0),
	m_color(255, 255, 255, 255)
{
    m_VBO = 0;
    m_IBO = 0;
}

Mesh::~Mesh()
{
    // Release the VBO and IBO
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
}

void Mesh::GenerateTriangle()
{
    // Vertex position info for a triangle.
    m_numOfVertices = 3;
    VertexFormat vertexattributes[] = {
        { 0.0f, 1.0f, 255, 0, 0, 255 },
        { -1.0f, -1.0f, 0, 255, 0, 255 },
        { 1.0f, -1.0f, 0, 0, 255, 255 },
    };

    GenerateShape(GL_TRIANGLES, vertexattributes, m_numOfVertices);
}

void Mesh::GenerateSquare()
{
    // Vertex position info for a triangle.
    m_numOfVertices = 4;
    VertexFormat vertexattributes[] = {
        { -1.0f, 1.0f, 255, 0, 0, 255 },
        { -1.0f, -1.0f, 0, 255, 0, 255 },
        { 1.0f, -1.0f, 255, 255, 255, 255 },
        { 1.0f, 1.0, 0, 0, 255, 255 },
    };

    m_numOfIndices = 4;
    unsigned int indices[] = {
        0, 1, 2, 3
        //2, 1, 3
    };

    GenerateShape(GL_TRIANGLE_FAN, vertexattributes, m_numOfVertices, indices, m_numOfIndices);
}

void Mesh::GenerateTextureSquare(vec2 spritesheetSize)
{
	// Vertex position info for a triangle.
	m_numOfVertices = 4;
	VertTexFormat vertexattributes[] = {
	{ -1.0f, 1.0f, 0.0f, 1.0f },
	{ -1.0f, -1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0, 1.0f, 1.0f },
	};

	m_numOfIndices = 4;
	unsigned int indices[] = {
		0, 1, 2, 3
		//2, 1, 3
	};

	m_spriteSize.Set(spritesheetSize.x, spritesheetSize.y);

	GenerateTexShape(GL_TRIANGLE_FAN, vertexattributes, m_numOfVertices, indices, m_numOfIndices);
}

void Mesh::GeneratePolygon(unsigned int sides, bool innerSides)
{
    std::vector<VertexFormat> vertices;

    // Intial point for circle is center for TRIANGLE FAN
    vertices.push_back(VertexFormat(0.0f, 0.0f, 255, 255, 255, 255));

    float initialOffset = TWOPI / 4;
    float innerSideOffset = TWOPI / (2 * sides);
    for (unsigned int i = 0; i < sides; ++i)
    {
        float angle = TWOPI * i / sides + initialOffset;
        VertexFormat vf(cosf(angle), sinf(angle), 255, 255, 255, 255);
        vertices.push_back(vf);

        if (innerSides)
        {
            VertexFormat vf(cosf(angle + innerSideOffset) / 2, sinf(angle + innerSideOffset) / 2, 255, 255, 255, 255);
            vertices.push_back(vf);
        }
    }

    m_numOfVertices = (unsigned int)vertices.size();

    std::vector<unsigned int> indices;
    // This should be fine since vertices will have
    // 2 + (sides * 2) number of sides
    unsigned int maxNumOfSides = innerSides ? (sides * 2) : sides;

    for (unsigned int i = 1; i <= maxNumOfSides; ++i)
    {
        indices.push_back(0); // Pivot for fan
        indices.push_back(i);   // Part of fan

        if (i + 1 < vertices.size())
            indices.push_back(i + 1);   // second part of fan
    }
    indices.push_back(1);

    m_numOfIndices = (unsigned int)indices.size();

    // Generate shape
    GenerateShape(GL_TRIANGLES, vertices.data(), m_numOfVertices, indices.data(), m_numOfIndices);
}

void Mesh::GenerateTexShape(GLenum mode, VertTexFormat vertexattributes[], unsigned int vertexNum, unsigned int *indexArray, unsigned int indexNum)
{
	// ATM this can only be called once, so we assert if it's called again with a VBO already allocated
	assert(m_VBO == 0);
	assert(m_IBO == 0);

	m_defaultMode = mode;
	m_numOfVertices = vertexNum;
	m_numOfIndices = indexNum;

	// Generate and fill buffer with our attributes.
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertTexFormat) * m_numOfVertices, vertexattributes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// OPTIONAL
	// Set Generate and fill index buffer
	if (indexArray != nullptr)
	{
		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numOfIndices, indexArray, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Check for errors.
	CheckForGLErrors();
}

void Mesh::GenerateShape(GLenum mode, VertexFormat vertexattributes[], unsigned int vertexNum, unsigned int *indexArray, unsigned int indexNum)
{
    // ATM this can only be called once, so we assert if it's called again with a VBO already allocated
    assert(m_VBO == 0);
    assert(m_IBO == 0);

    m_defaultMode = mode;
    m_numOfVertices = vertexNum;
    m_numOfIndices = indexNum;

    // Generate and fill buffer with our attributes.
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_numOfVertices, vertexattributes, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // OPTIONAL
    // Set Generate and fill index buffer
    if (indexArray != nullptr)
    {
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numOfIndices, indexArray, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // Check for errors.
    CheckForGLErrors();
}

void Mesh::Draw(vec2 shapeScale, vec2 shapePos, vec2 cameraPos, vec2 projectionScale, ShaderProgram *pShader)
{
    // Call Draw using default drawmode
    Draw(shapeScale, shapePos, cameraPos, projectionScale, pShader, m_defaultMode);
}

void Mesh::Draw(vec2 shapeScale, vec2 shapePos, vec2 cameraPos, vec2 projectionScale, ShaderProgram *pShader, GLenum mode)
{
    // Use Shader
    glUseProgram(pShader->GetProgram());

    // Still need to bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // ----------------UNIFORMS-------------------
    GLint uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ModelScale");
    if (uLoc != -1)
    {
        glUniform2f(uLoc, shapeScale.x, shapeScale.y);
    }

    uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ModelTranslation");
    if (uLoc != -1)
    {
        glUniform2f(uLoc, shapePos.x, shapePos.y);
    }

    uLoc = glGetUniformLocation(pShader->GetProgram(), "u_CameraPos");
    if (uLoc != -1)
    {
        glUniform2f(uLoc, cameraPos.x, cameraPos.y);
    }

    uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ProjectionScale");
    if (uLoc != -1)
    {
        glUniform2f(uLoc, projectionScale.x, projectionScale.y);
    }

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_Color");
	if (uLoc != -1)
	{
		float charSize = 255;
		float r = (float)m_color.r / charSize;
		glUniform4f(uLoc, (float)m_color.r / charSize, (float)m_color.g / charSize, (float)m_color.b / charSize, (float)m_color.a / charSize);
	}

    // -------------------ATTRIBUTES--------------------
    // Get the index of the position attribute in our shader and setup the attribute details.
    GLint loc = glGetAttribLocation(pShader->GetProgram(), "a_Position");
    if (loc != -1)
    {
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
        glEnableVertexAttribArray(loc);
    }

    loc = glGetAttribLocation(pShader->GetProgram(), "a_Color");
    if (loc != -1)
    {
        glVertexAttribPointer(loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)sizeof(vec2));
        glEnableVertexAttribArray(loc);
    }

    // Draw the triangle.
    if (m_IBO == 0)
    {
        glDrawArrays(mode, 0, m_numOfVertices);
    }
    else
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glDrawElements(mode, m_numOfIndices, GL_UNSIGNED_INT, (void*)0);
    }

    // Check for errors.
    CheckForGLErrors();
}

void Mesh::DrawTexture(vec2 shapeScale, vec2 shapePos, vec2 cameraPos, vec2 projectionScale, ShaderProgram *pShader, GLuint tex)
{
	// Use Shader
	glUseProgram(pShader->GetProgram());

	// ----------------UNIFORMS-------------------
	GLint uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ModelScale");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, shapeScale.x, shapeScale.y);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ModelTranslation");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, shapePos.x, shapePos.y);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_CameraPos");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, cameraPos.x, cameraPos.y);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ProjectionScale");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, projectionScale.x, projectionScale.y);
	}


	glBindTexture(GL_TEXTURE_2D, tex);
	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_Texture");
	if (uLoc != -1)
	{
		glUniform1i(uLoc, 0);
	}

	// -------------------ATTRIBUTES--------------------
	// Get the index of the position attribute in our shader and setup the attribute details.
	// Still need to bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	GLint loc = glGetAttribLocation(pShader->GetProgram(), "a_Position");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertTexFormat), (void*)0);
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(pShader->GetProgram(), "a_UVCoord");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertTexFormat), (void*)sizeof(vec2));
		glEnableVertexAttribArray(loc);
	}

	// Draw the triangle.
	if (m_IBO == 0)
	{
		glDrawArrays(m_defaultMode, 0, m_numOfVertices);
		CheckForGLErrors();
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glDrawElements(m_defaultMode, m_numOfIndices, GL_UNSIGNED_INT, (void*)0);
		CheckForGLErrors();
	}

	// Check for errors.
	CheckForGLErrors();
}

void Mesh::DrawSprite(vec2 shapeScale, vec2 shapePos, vec2 cameraPos, vec2 projectionScale, ShaderProgram *pShader, GLuint tex, SpriteFormat sprite)
{
	// Use Shader
	glUseProgram(pShader->GetProgram());

	// ----------------UNIFORMS-------------------
	GLint uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ModelScale");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, shapeScale.x, shapeScale.y);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ModelTranslation");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, shapePos.x, shapePos.y);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_CameraPos");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, cameraPos.x, cameraPos.y);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_ProjectionScale");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, projectionScale.x, projectionScale.y);
	}


	// ----------------------TEXTURE-------------------------------
	glBindTexture(GL_TEXTURE_2D, tex);
	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_Texture");
	if (uLoc != -1)
	{
		glUniform1i(uLoc, 0);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_uvOffset");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, sprite.uvOffset.x, sprite.uvOffset.y);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_uvSize");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, sprite.uvSize.x, sprite.uvSize.y);
	}

	uLoc = glGetUniformLocation(pShader->GetProgram(), "u_spritesheetSize");
	if (uLoc != -1)
	{
		glUniform2f(uLoc, m_spriteSize.x, m_spriteSize.y);
	}


	// -------------------ATTRIBUTES--------------------
	// Get the index of the position attribute in our shader and setup the attribute details.
	// Still need to bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	GLint loc = glGetAttribLocation(pShader->GetProgram(), "a_Position");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertTexFormat), (void*)0);
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(pShader->GetProgram(), "a_UVCoord");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertTexFormat), (void*)sizeof(vec2));
		glEnableVertexAttribArray(loc);
	}

	// Draw the triangle.
	if (m_IBO == 0)
	{
		glDrawArrays(m_defaultMode, 0, m_numOfVertices);
		CheckForGLErrors();
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glDrawElements(m_defaultMode, m_numOfIndices, GL_UNSIGNED_INT, (void*)0);
		CheckForGLErrors();
	}

	// Check for errors.
	CheckForGLErrors();
}

void Mesh::SetColor(ColorStruct color)
{
	m_color = color;
}

ColorStruct Mesh::GetColor()
{
	return m_color;
}

void Mesh::SetSpriteSize(vec2 size)
{
	m_spriteSize = size;
}
