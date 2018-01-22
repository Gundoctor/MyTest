#ifndef __PLAYER_H__
#define __PLAYER_H__

#define PLAYER_SIZE  80.0f
#define PLAYER_SPEED 200.0f

class PlayerController;
class GameSample;

// Bad for now
class Tilemap;

class Player : public GameObject
{
public:
    Player(vec2 pos, float size, GameSample* gameSample);
    virtual ~Player();

    void SetGameSample(GameSample* gameSample);
	void SetTilemap(Tilemap* tilemap);
    PlayerController* GetController();

protected:
	void UpdateCurrent(float deltatime) override;

private:
	PlayerController *m_pController;
	GameSample *m_pGameSample;

	Tilemap* m_pTilemap;
};

#endif