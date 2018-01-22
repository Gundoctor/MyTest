#include "GamePCH.h"

ControllableCamera::ControllableCamera(vec2 pos, vec2 proj, vec2 vpCoord, vec2 vpSize) :
    Camera(GameObjectType::Camera, pos, proj, vpCoord, vpSize),
    m_controller(nullptr)
{
    m_controller = new CameraController();
}

ControllableCamera::~ControllableCamera()
{
    if (m_controller != nullptr)
    {
        delete m_controller;
        m_controller = nullptr;
    }
}

void ControllableCamera::UpdateCurrent(float deltatime)
{
    unsigned int flags = m_controller->GetCurrentFlags();
    vec2 dirVec(0.0f, 0.0f);
    vec2 projectionMod(0.0f, 0.0f);

    if (flags & CAMERA_LEFT)
    {
        dirVec.x -= CAMERA_SPEED;
    }
    if (flags & CAMERA_RIGHT)
    {
        dirVec.x += CAMERA_SPEED;
    }
    if (flags & CAMERA_UP)
    {
        dirVec.y += CAMERA_SPEED;
    }
    if (flags & CAMERA_DOWN)
    {
        dirVec.y -= CAMERA_SPEED;
    }
    if (flags & CAMERA_ZOOM_IN)
    {
		// For uniform scaling, we boost the speed of the larger component
		float minComponent = min(m_projection.x, m_projection.y);

        projectionMod.x -= CAMERA_ZOOM_SPEED * (m_projection.x / minComponent);
		projectionMod.y -= CAMERA_ZOOM_SPEED * (m_projection.y / minComponent);
    }
    if (flags & CAMERA_ZOOM_OUT)
    {
		// For uniform scaling, we boost the speed of the larger component 
		float minComponent = min(m_projection.x, m_projection.y);

		projectionMod.x += CAMERA_ZOOM_SPEED * (m_projection.x / minComponent);
		projectionMod.y += CAMERA_ZOOM_SPEED * (m_projection.y / minComponent);
    }

    if (m_pFollowTarget != nullptr)
    {
        m_position = m_pFollowTarget->GetPosition();
    }
    else
    {
        dirVec.Normalize();
        m_position += dirVec * CAMERA_SPEED * deltatime;
    }

    m_projection += projectionMod * deltatime;
}

void ControllableCamera::OnEvent(InputEvent *pEvent)
{
    m_controller->OnEvent(pEvent);
}