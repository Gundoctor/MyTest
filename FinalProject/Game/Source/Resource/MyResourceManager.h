#ifndef __SAMPLEMESHMANAGER_H__
#define __SAMPLEMESHMANAGER_H__


#define CICLEPOINTS     50
#define DIAMONDPOINTS   4
#define PENTAPOINTS     5
#define HEXAPOINTS      6
#define STARPOINTS      7


namespace MeshType
{
    enum MeshType : unsigned int
    {
        Circle, 
        Hexagon,
        Triangle,
        Square,
        Star,
        Pentagon,
		TexSquare
    };
}

namespace ShaderName
{
	enum ShaderName : unsigned int
	{
		Default,
		Minimap,
		Texture,
		Particle,
		Score,
		Healthbar
	};
}

namespace TexType
{
	enum TexType : unsigned int
	{
		Player,
		Fire,
		ScoreStrip,
		BombermanSpriteSheet,
		MAX_NUM
	};
}

namespace TileType
{
	enum TileType : unsigned int
	{
		Grass,
		Water,
		Wall,
		Start,
		End,
		UNWALKABLE,
		MAX_NUM
	};
}


class MyResourceManager : public ResourceManager
{
public:
    MyResourceManager();
    ~MyResourceManager();

	virtual void AddMesh(MeshType::MeshType id, Mesh* mesh);
	virtual void AddShader(ShaderName::ShaderName id, ShaderProgram* shader);
	virtual void GenerateTexture(TexType::TexType id, const std::string filepath);

	virtual Mesh * GetMesh(MeshType::MeshType id) const;
	virtual ShaderProgram* GetShader(ShaderName::ShaderName id) const;
	virtual GLuint GetTexture(TexType::TexType id) const;
};


#endif