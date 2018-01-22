#include "FrameworkPCH.h"

GameCore::GameCore(Framework* pFramework)
{
    m_pFramework = pFramework;

	m_gameStartTime = GetSystemTime();
}

GameCore::~GameCore()
{
}

void GameCore::SetAspectRatio(int width, int height)
{
	m_aspectRatio.Set(width, height);
}

Vector2Int GameCore::GetAspectRatio()
{
	return m_aspectRatio;
}

Point GameCore::GetWindowSize() const
{
    assert(m_pFramework != nullptr);
    
    return m_pFramework->GetWindowSize();
}

Point GameCore::GetPreviousWindowSize() const
{
	assert(m_pFramework != nullptr);

	return m_pFramework->GetPreviousWindowSize();
}

double GameCore::GetTimeSinceStart()
{
	double currentTime = GetSystemTime();
	
	return (currentTime - m_gameStartTime);
}

Vector2Int GameCore::GetMouseCoordinates()
{
	int x, y;
	m_pFramework->GetMouseCoordinates(&x, &y);

	Vector2Int mouseCoord(x, y);

	return mouseCoord;
}