#include "GamePCH.h"

MyResourceManager::MyResourceManager()
{

}

MyResourceManager::~MyResourceManager()
{
}


void MyResourceManager::AddMesh(MeshType::MeshType id, Mesh* mesh)
{
	ResourceManager::AddMesh(id, mesh);
}

void MyResourceManager::AddShader(ShaderName::ShaderName id, ShaderProgram* shader)
{
	ResourceManager::AddShader(id, shader);
}

void MyResourceManager::GenerateTexture(TexType::TexType id, const std::string filepath)
{
	ResourceManager::GenerateTexture(id, filepath);
}

Mesh* MyResourceManager::GetMesh(MeshType::MeshType type) const
{
    return ResourceManager::GetMesh(type);
}

ShaderProgram* MyResourceManager::GetShader(ShaderName::ShaderName id) const
{
	return ResourceManager::GetShader(id);
}

GLuint MyResourceManager::GetTexture(TexType::TexType id) const
{
	return ResourceManager::GetTexture(id);
}