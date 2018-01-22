#ifndef __MESH_H__
#define __MESH_H__

class ShaderProgram;

#define TWOPI 6.28f

struct ColorStruct
{
	unsigned char r, g, b, a;

	ColorStruct()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
	ColorStruct(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

struct VertexFormat
{
    vec2 Position; 
	ColorStruct Color;

    VertexFormat(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        Position.x = x;
        Position.y = y;
        Color.r = r;
        Color.g = g;
        Color.b = b;
        Color.a = a;
    }
};

struct SpriteFormat
{
	vec2 uvOffset;
	vec2 uvSize;
};

struct VertTexFormat
{
	Vector2 Position;
	Vector2 UVPos;

	VertTexFormat(float x, float y, float u, float v)
	{
		Position.x = x;
		Position.y = y;
		UVPos.x = u;
		UVPos.y = v;
	}
};

class Mesh
{
private:
    GLuint m_VBO;
    GLuint m_IBO;

    GLenum m_defaultMode;

    unsigned int m_numOfVertices;
    unsigned int m_numOfIndices;
	
	ColorStruct m_color;
	vec2 m_spriteSize;

public:
    Mesh();
    virtual ~Mesh();

    // Generate functions
    void GenerateTriangle();
    void GenerateSquare();
	void GenerateTextureSquare(vec2 spritesheetSize);
    void GeneratePolygon(unsigned int sides, bool innerSides = false);
	void GenerateTexShape(GLenum mode, VertTexFormat vf[], unsigned int vertexNum, unsigned int *indexArray = nullptr, unsigned int indexNum = 0);
    void GenerateShape(GLenum mode, VertexFormat vf[], unsigned int vertexNum, unsigned int *indexArray = nullptr, unsigned int indexNum = 0);

    // Draw functions
    void Draw(vec2 shapeScale, vec2 shapePos, vec2 cameraPos, vec2 projectionScale, ShaderProgram *pShader);
    void Draw(vec2 shapeScale, vec2 shapePos, vec2 cameraPos, vec2 projectionScale, ShaderProgram *pShader, GLenum mode);
	void DrawTexture(vec2 shapeScale, vec2 shapePos, vec2 cameraPos, vec2 projectionScale, ShaderProgram *pShader, GLuint tex);

	// I'm sorry for this one
	void DrawSprite(vec2 shapeScale, vec2 shapePos, vec2 cameraPos, vec2 projectionScale, ShaderProgram *pShader, GLuint tex, SpriteFormat sprite);

    // Getters
	ColorStruct GetColor();

    // Setters
    void SetColor(ColorStruct color);
	void SetSpriteSize(vec2 size);
};


#endif