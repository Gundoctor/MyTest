#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#define PLAYER_UP       0x0001
#define PLAYER_DOWN     0x0002
#define PLAYER_RIGHT    0x0004
#define PLAYER_LEFT     0x0008
#define PLAYER_JUMP     0x0010

class PlayerController
{
public:
    PlayerController();
    ~PlayerController();

    bool OnEvent(InputEvent* pEvent);
    unsigned int GetCurrentFlags();

    vec2 GetMovementValues();


private:
    unsigned int m_currentFlags;
    unsigned int m_newFlags;

    vec2 m_movementValues;
};

#endif