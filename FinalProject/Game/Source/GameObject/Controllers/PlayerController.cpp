#include "GamePCH.h"


PlayerController::PlayerController() :
    m_currentFlags(0), 
    m_newFlags(0),
    m_movementValues(0.0f, 0.0f)
{
}

PlayerController::~PlayerController()
{
}

bool PlayerController::OnEvent(InputEvent* pEvent)
{
	bool processedEvent = false;

    if (pEvent->GetDeviceType() == DeviceType::Keyboard)
    {
        if (pEvent->GetKeyState() == KeyState::Pressed)
        {
            if (pEvent->GetKeyID() == VK_DOWN)
            {
                m_newFlags |= PLAYER_DOWN;
            }
            if (pEvent->GetKeyID() == VK_UP)
            {
                m_newFlags |= PLAYER_UP;
            }
            if (pEvent->GetKeyID() == VK_LEFT)
            {
                m_newFlags |= PLAYER_LEFT;
            }
            if (pEvent->GetKeyID() == VK_RIGHT)
            {
                m_newFlags |= PLAYER_RIGHT;
            }
        }
        else if (pEvent->GetKeyState() == KeyState::Released)
        {
            if (pEvent->GetKeyID() == VK_DOWN)
            {
                m_newFlags &= ~PLAYER_DOWN;
            }
            if (pEvent->GetKeyID() == VK_UP)
            {
                m_newFlags &= ~PLAYER_UP;
            }
            if (pEvent->GetKeyID() == VK_LEFT)
            {
                m_newFlags &= ~PLAYER_LEFT;
            }
            if (pEvent->GetKeyID() == VK_RIGHT)
            {
                m_newFlags &= ~PLAYER_RIGHT;
            }
        }

        // Check if we did any processing
        if (m_currentFlags ^ m_newFlags)
        {
            processedEvent = true;

            // Adjust movement values
            m_movementValues.x = 1.0f;
            m_movementValues.y = 1.0f;
        }
    }


    // something something new vs current flags
    m_currentFlags = m_newFlags;

	return processedEvent;
}

unsigned int PlayerController::GetCurrentFlags()
{
    return m_currentFlags;
}

vec2 PlayerController::GetMovementValues()
{
    return m_movementValues;
}