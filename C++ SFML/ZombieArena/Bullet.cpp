#include "pch.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

Bullet::Bullet()
{
	m_BulletShape.setSize(Vector2f(2, 2));
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	m_InFlight = 1;
	m_Position.x = startX;
	m_Position.y = startY;
	
	float vectorY = targetY - startY, vectorX = targetX - startX, hypotenuse = sqrt(vectorY * vectorY + vectorX * vectorX);

	m_BulletDistanceX = vectorX / hypotenuse * m_BulletSpeed;
	m_BulletDistanceY = vectorY / hypotenuse * m_BulletSpeed;

	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	m_BulletShape.setPosition(m_Position);
}

void Bullet::stop()
{
	m_InFlight = 0;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

void Bullet::update(float elapsedTime)
{
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;
	
	m_BulletShape.setPosition(m_Position);

	if (m_Position.x < m_MinX || m_Position.x > m_MaxX || m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = 0;
	}
}