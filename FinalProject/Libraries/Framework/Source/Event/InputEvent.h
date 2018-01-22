#ifndef __INPUTEVENT_H__
#define __INPUTEVENT_H__

namespace DeviceType
{
    enum DeviceType
    {
        Keyboard,
        Mouse,
        Joystick,
        Joypad
    };
}

namespace KeyState
{
    enum KeyState
    {
        Pressed,
        Held,
        Released
    };
}

namespace GamepadID
{
    enum GamepadID
    {
        Up,
        Left,
        Down,
        Right,
        A, 
        B
    };
}


class InputEvent : public Event
{
public:
    InputEvent(DeviceType::DeviceType, KeyState::KeyState, int, float, float);
    InputEvent(DeviceType::DeviceType, KeyState::KeyState, GamepadID::GamepadID, float, float);
    virtual ~InputEvent();

    // GETTERS
    DeviceType::DeviceType  GetDeviceType();
    KeyState::KeyState      GetKeyState();
    int                     GetKeyID();
    GamepadID::GamepadID    GetGamepadID();
    vec2                    GetCoord();

private:
    // Important member vars
    DeviceType::DeviceType  m_deviceType;
    KeyState::KeyState      m_keyState;
    vec2                    m_coord;

    union
    {
        int                     m_keyID;
        GamepadID::GamepadID    m_gamepadID;
    };
};

#endif