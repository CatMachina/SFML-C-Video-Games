#include "pch.h"
#include "zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Zombie::spawn(float startX, float startY, int type, int seed)
{
	switch(type)
	{
	case 0:
		// Bloater
		m_Sprite = Sprite(TextureHolder::getTexture("graphics/bloater.png"));
		m_Speed = BLOATER_SPEED;
		m_Health = BLOATER_HEALTH;
		break;
	case 1:
		// Chaser
		m_Sprite = Sprite(TextureHolder::getTexture("graphics/chaser.png"));
		m_Speed = CHASER_SPEED;
		m_Health = CHASER_HEALTH;
		break;
	case 2:
		// Crawler
		m_Sprite = Sprite(TextureHolder::getTexture("graphics/crawler.png"));
		m_Speed = CRAWLER_SPEED;
		m_Health = CRAWLER_HEALTH;
		break;
	}
	srand((int)time(0) * seed);
	float modifier = (rand() % MAX_VARIANCE) + OFFSET;
	modifier /= 100;
	m_Speed *= modifier;
	m_Position.x = startX, m_Position.y = startY;
	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
}

bool Zombie::hit()
{
	m_Health--;
	if(m_Health <= 0)
	{
		m_Alive = 0;
		m_Sprite.setTexture(TextureHolder::getTexture("graphics/blood.png"));
		return 1;
	}
	return 0;
}

bool Zombie::isAlive()
{
	return m_Alive;
}

FloatRect Zombie::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Zombie::getSprite()
{
	return m_Sprite;
}

void Zombie::update(float elapsedTime, Vector2f playerLocation)
{
	float playerX = playerLocation.x, playerY = playerLocation.y;
	bool xd = playerX < m_Position.x, yd  = playerY < m_Position.y;
	m_Position.x += m_Speed * elapsedTime * (xd ? -1 : 1);
	m_Position.y += m_Speed * elapsedTime * (yd ? -1 : 1);
	m_Sprite.setPosition(m_Position);
	float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x) * 180 / 3.14159265);
	m_Sprite.setRotation(angle);
}