#include "pch.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private:
	const int HEALTH_START_VALUE = 50, AMMO_START_VALUE = 12, START_WAIT_TIME = 10, START_SECONDS_TO_LIVE = 5;

	Sprite m_Sprite;
	IntRect m_Arena;
	int m_Value, m_Type;
	bool m_Spawned;
	float m_SecondsSinceSpawn, m_SecondsSinceDespawn, m_SecondsToLive, m_SecondsToWait;
	
public:
	Pickup(int type);
	void setArena(IntRect arena);
	void spawn();
	FloatRect getPosition();
	Sprite getSprite();
	void update(float elapsedTime);
	bool isSpawned();
	int gotIt();
	void upgrade();
	void reset(int type);
};