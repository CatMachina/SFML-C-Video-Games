#include "pch.h"
#include "Player.h"
#include "TextureHolder.h"
#include <algorithm>

Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	m_Sprite = Sprite(TextureHolder::getTexture("graphics/player.png"));

	m_Sprite.setOrigin(25, 25);
	m_UpPressed = 0;
	m_DownPressed = 0;
	m_LeftPressed = 0;
	m_RightPressed = 0;
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	m_Arena.top = arena.top;
	m_Arena.height = arena.height;
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;

	m_TileSize = tileSize;

	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

Time Player::getLastHitTime()
{
	return m_LastHit;
}

bool Player::hit(Time timeHit) 
{
	if(timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return 1;
	}
	else 
	{
		return 0;
	}
}

FloatRect Player::getPosition() 
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation() 
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

int Player::getHealth()
{
	return m_Health;
}

void Player::moveUp() 
{
	m_UpPressed = 1;
	return;
}

void Player::moveDown()
{
	m_DownPressed = 1;
	return;
}

void Player::moveLeft()
{
	m_LeftPressed = 1;
	return;
}

void Player::moveRight()
{
	m_RightPressed = 1;
	return;
}

void Player::stopUp()
{
	m_UpPressed = 0;
	return;
}

void Player::stopDown()
{
	m_DownPressed = 0;
	return;
}

void Player::stopLeft()
{
	m_LeftPressed = 0;
	return;
}

void Player::stopRight()
{
	m_RightPressed = 0;
	return;
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
	if(m_UpPressed)
	{
		m_Position.y -= elapsedTime * m_Speed;
	}
	if(m_DownPressed)
	{
		m_Position.y += elapsedTime * m_Speed;
	}
	if(m_LeftPressed)
	{
		m_Position.x -= elapsedTime * m_Speed;
	}
	if(m_RightPressed)
	{
		m_Position.x += elapsedTime * m_Speed;
	}
	m_Sprite.setPosition(m_Position.x, m_Position.y);

	m_Position.x = std::min(m_Position.x, (float)m_Arena.width - m_TileSize);
	m_Position.x = std::max(m_Position.x, (float)m_Arena.left + m_TileSize);
	m_Position.y = std::min(m_Position.y, (float)m_Arena.height - m_TileSize);
	m_Position.y = std::max(m_Position.y, (float)m_Arena.top + m_TileSize);

	float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2) * 180 / 3.14159265);
	m_Sprite.setRotation(angle);
	return;
}

void Player::upgradeSpeed()
{
	m_Speed += START_SPEED * 0.2f;
	return;
}

void Player::upgradeHealth()
{
	m_MaxHealth += START_HEALTH * 0.2f;
	return;
}

void Player::increaseHealthLevel(int amount) 
{
	m_Health = std::min(m_MaxHealth, m_Health + amount);
	return;
}