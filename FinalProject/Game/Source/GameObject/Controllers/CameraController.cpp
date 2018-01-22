#include "GamePCH.h"


CameraController::CameraController() :
    m_currentFlags(0)
{
}

CameraController::~CameraController()
{
}

void CameraController::OnEvent(InputEvent *pEvent)
{
    if (pEvent->GetDeviceType() == DeviceType::Keyboard)
    {
        if (pEvent->GetKeyState() == KeyState::Pressed)
        {
            if (pEvent->GetKeyID() == 'A')
            {
                m_currentFlags |= CAMERA_LEFT;
            }
            if (pEvent->GetKeyID() == 'D')
            {
                m_currentFlags |= CAMERA_RIGHT;
            }
            if (pEvent->GetKeyID() == 'W')
            {
                m_currentFlags |= CAMERA_UP;
            }
            if (pEvent->GetKeyID() == 'S')
            {
                m_currentFlags |= CAMERA_DOWN;
            }
            if (pEvent->GetKeyID() == 'Q')
            {
                m_currentFlags |= CAMERA_ZOOM_OUT;
            }
            if (pEvent->GetKeyID() == 'E')
            {
                m_currentFlags |= CAMERA_ZOOM_IN;
            }
        }
        else if (pEvent->GetKeyState() == KeyState::Released)
        {
            if (pEvent->GetKeyID() == 'A')
            {
                m_currentFlags &= ~CAMERA_LEFT;
            }
            if (pEvent->GetKeyID() == 'D')
            {
                m_currentFlags &= ~CAMERA_RIGHT;
            }
            if (pEvent->GetKeyID() == 'W')
            {
                m_currentFlags &= ~CAMERA_UP;
            }
            if (pEvent->GetKeyID() == 'S')
            {
                m_currentFlags &= ~CAMERA_DOWN;
            }
            if (pEvent->GetKeyID() == 'Q')
            {
                m_currentFlags &= ~CAMERA_ZOOM_OUT;
            }
            if (pEvent->GetKeyID() == 'E')
            {
                m_currentFlags &= ~CAMERA_ZOOM_IN;
            }
        }
    }
}

unsigned int CameraController::GetCurrentFlags()
{
    return m_currentFlags;
}