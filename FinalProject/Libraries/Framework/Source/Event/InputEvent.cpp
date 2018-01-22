#include "FrameworkPCH.h"
#include "InputEvent.h"


InputEvent::InputEvent(DeviceType::DeviceType deviceType, KeyState::KeyState keyState, int keyID, float x, float y) :
    Event(EventType::Input), m_deviceType(deviceType), 
    m_keyState(keyState), m_keyID(keyID),
    m_coord(vec2(x, y))
{
}

InputEvent::InputEvent(DeviceType::DeviceType deviceType, KeyState::KeyState keyState, GamepadID::GamepadID gamepadButtonID, float x, float y) :
    Event(EventType::Input), m_deviceType(deviceType),
    m_keyState(keyState), m_gamepadID(gamepadButtonID),
    m_coord(vec2(x, y))
{
}

InputEvent::~InputEvent()
{
    int x = 4;
}


DeviceType::DeviceType InputEvent::GetDeviceType()
{
    return m_deviceType;
}

int InputEvent::GetKeyID()
{
    return m_keyID;
}

GamepadID::GamepadID InputEvent::GetGamepadID()
{
    return m_gamepadID;
}

KeyState::KeyState InputEvent::GetKeyState()
{
    return m_keyState;
}

vec2 InputEvent::GetCoord()
{
    return m_coord;
}