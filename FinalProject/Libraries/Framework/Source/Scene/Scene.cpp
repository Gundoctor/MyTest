#include "FrameworkPCH.h"

Scene::Scene() :
	m_pRoot(nullptr)
{
}

Scene::~Scene()
{
}

bool Scene::OnEvent(Event *pEvent)
{
	return false;
}

void Scene::UpdateCameraViewports(const GameCore* game)
{

}