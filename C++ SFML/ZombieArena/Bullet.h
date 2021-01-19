#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:

	Vector2f m_Position;
	RectangleShape m_BulletShape;
	bool m_InFlight;
	float m_BulletSpeed = 1000;
	float m_BulletDistanceX, m_BulletDistanceY;
	float m_MaxX, m_MinX, m_MaxY, m_MinY;

public:

	Bullet();
	void stop();
	bool isInFlight();
	void shoot(float startX, float startY, float targetX, float targetY);
	FloatRect getPosition();
	RectangleShape getShape();
	void update(float elapsedTime);

};