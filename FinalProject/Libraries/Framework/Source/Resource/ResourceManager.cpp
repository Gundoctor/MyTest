#include "FrameworkPCH.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	// Delete meshes
	for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		delete it->second;
	}
	m_meshes.clear();

	// Delete shaders
	for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		delete it->second;
	}
	m_shaders.clear();

	// Delete textures
	for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		glDeleteTextures(1, &it->second);
	}
	m_textures.clear();
}

int ResourceManager::AddMesh(unsigned int id, Mesh* mesh)
{
    if (m_meshes.find(id) != m_meshes.end())
        return -1;
    
	m_meshes.insert(std::pair<unsigned int, Mesh*>(id, mesh));
    return 0;
}

Mesh* ResourceManager::GetMesh(unsigned int id) const
{
    if (m_meshes.find(id) == m_meshes.end())
        return nullptr;

    return m_meshes.at(id);
}

int ResourceManager::AddShader(unsigned int id, ShaderProgram* shader)
{
    if (m_shaders.find(id) != m_shaders.end())
        return -1;

	m_shaders.insert(std::pair<unsigned int, ShaderProgram*>(id, shader));
    return 0;
}

ShaderProgram* ResourceManager::GetShader(unsigned int id) const
{
	if (m_shaders.find(id) == m_shaders.end())
		return nullptr;

	return m_shaders.at(id);
}

int ResourceManager::GenerateTexture(unsigned int id, std::string filepath)
{
    if (m_textures.find(id) != m_textures.end())
        return -1;

	GLuint m_TexID = LoadTexture(filepath.c_str());
	m_textures.insert(std::pair<unsigned int, GLuint>(id, m_TexID));
    return 0;
}

GLuint ResourceManager::GetTexture(unsigned int id) const
{
	if (m_textures.find(id) == m_textures.end())
		return 0;

	return m_textures.at(id);
}