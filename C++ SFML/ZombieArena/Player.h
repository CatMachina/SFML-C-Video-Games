#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200, START_HEALTH = 100;
	// Where is player?
	Vector2f m_Position;
	// Player graphics
	Sprite m_Sprite;
	Texture m_Texture;
	// Resolution?
	Vector2f m_Resolution;
	// Current Arena size
	IntRect m_Arena;
	// Tile size
	int m_TileSize;
	// Active Directions
	bool m_UpPressed, m_DownPressed, m_LeftPressed, m_RightPressed;
	// Player details
	int m_Health, m_MaxHealth;
	Time m_LastHit;
	float m_Speed;

public:
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	// Call at end of game
	void resetPlayerStats();
	// Hit by zombie
	bool hit(Time timeHit);
	// Invincibility time
	Time getLastHitTime();
	// Player location
	FloatRect getPosition();
	// Center of player
	Vector2f getCenter();
	// Angle of player
	float getRotation();
	// Send player graphics
	Sprite getSprite();
	// Player movement
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	// Stop player movement
	void stopUp();
	void stopDown();
	void stopLeft();
	void stopRight();
	// Update in real-time
	void update(float elaspedTime, Vector2i mousePosition);
	// Boosts
	void upgradeSpeed();
	void upgradeHealth();
	void increaseHealthLevel(int amount);
	// Remaining health
	int getHealth();

};