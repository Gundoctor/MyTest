#ifndef __GAMEEVENT_H__
#define __GAMEEVENT_H__

namespace GameEventType
{
    enum GameEventType
    {
		ChangeTilemap
    };
}

class Game1Scene;

class GameEvent : public Event
{
private:
    GameEventType::GameEventType m_gameEventType;
	unsigned int m_tilemapNum;

public:
    GameEvent(GameEventType::GameEventType type, unsigned int tnum);
    virtual ~GameEvent();

    GameEventType::GameEventType GetGameEventType();

	unsigned int GetTilemapNum();
};


#endif
