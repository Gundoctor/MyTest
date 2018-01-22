#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

protected:
    // Protected Methods
	virtual int AddMesh(unsigned int id, Mesh* mesh);
	virtual int AddShader(unsigned int id, ShaderProgram* shader);
	virtual int GenerateTexture(unsigned int id, const std::string filepath);

    virtual Mesh* GetMesh(unsigned int id) const;
	virtual ShaderProgram* GetShader(unsigned int id) const;
	virtual GLuint GetTexture(unsigned int id) const;


    // Protected Member Variables
    std::map<unsigned int, Mesh*> m_meshes;
	std::map<unsigned int, ShaderProgram*> m_shaders;
	std::map<unsigned int, GLuint> m_textures;

};

#endif