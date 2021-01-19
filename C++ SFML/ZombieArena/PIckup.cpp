#include "pch.h"
#include "Pickup.h"
#include "TextureHolder.h"
#include <SFML/Graphics.hpp>

Pickup::Pickup(int type)
{
	m_Type = type;
	m_Sprite = m_Type == 1 ? Sprite(TextureHolder::getTexture("graphics/health_pickup.png")) : Sprite(TextureHolder::getTexture("graphics/ammo_pickup.png"));
	m_Sprite.setOrigin(25, 25);
	m_Value = m_Type == 1 ? HEALTH_START_VALUE : AMMO_START_VALUE;
	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsToWait = START_WAIT_TIME;
}

void Pickup::setArena(IntRect arena)
{
	m_Arena.left = arena.left + 50;
	m_Arena.width = arena.width - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.height = arena.height - 50;
	spawn();
}

void Pickup::spawn()
{
	srand((int)time(0) / m_Type);
	int x = (rand() % m_Arena.width);
	srand((int)time(0) * m_Type);
	int y = (rand() % m_Arena.height);
	m_SecondsSinceSpawn = 0;
	m_Spawned = 1;
	m_Sprite.setPosition(x, y);
}

FloatRect Pickup::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Pickup::getSprite()
{
	return m_Sprite;
}

bool Pickup::isSpawned()
{
	return m_Spawned;
}

int Pickup::gotIt()
{
	m_Spawned = 0;
	m_SecondsSinceDespawn = 0;
	return m_Value;
}

void Pickup::update(float elapsedTime)
{
	if (m_Spawned)
	{
		m_SecondsSinceSpawn += elapsedTime;
	}
	else
	{
		m_SecondsSinceDespawn += elapsedTime;
	}
	if (m_SecondsSinceSpawn > m_SecondsToLive && m_Spawned)
	{
		m_Spawned = 0;
		m_SecondsSinceDespawn = 0;
	}

	if (m_SecondsSinceDespawn > m_SecondsToWait && !m_Spawned)
	{
		spawn();
	}
}

void Pickup::upgrade()
{
	m_Value += (m_Type == 1 ? HEALTH_START_VALUE * 0.5 : AMMO_START_VALUE * 0.5);
	m_SecondsToLive += START_SECONDS_TO_LIVE / 10;
	m_SecondsToWait -= START_WAIT_TIME / 10;
}

void Pickup::reset(int type)
{
	m_Value = type == 1 ? HEALTH_START_VALUE : AMMO_START_VALUE;
	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsToWait = START_WAIT_TIME;
}