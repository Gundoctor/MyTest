#include "GamePCH.h"


GameEvent::GameEvent(GameEventType::GameEventType type, unsigned int tnum) :
    Event(EventType::Game),
    m_gameEventType(type),
	m_tilemapNum(tnum)
{
}

GameEvent::~GameEvent()
{
}


GameEventType::GameEventType GameEvent::GetGameEventType()
{
    return m_gameEventType;
}

unsigned int GameEvent::GetTilemapNum()
{
	return m_tilemapNum;
}