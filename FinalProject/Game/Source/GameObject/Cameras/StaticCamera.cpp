#include "GamePCH.h"

StaticCamera::StaticCamera(vec2 pos, vec2 proj, vec2 vpCoord, vec2 vpSize) : 
    Camera(GameObjectType::Camera, pos, proj, vpCoord, vpSize)
{
    m_drawMode = GL_LINE_LOOP;
}

StaticCamera::~StaticCamera()
{
}

void StaticCamera::UpdateCurrent(float deltatime)
{
    if (m_pFollowTarget != nullptr)
    {
        m_position = m_pFollowTarget->GetPosition();
    }
}