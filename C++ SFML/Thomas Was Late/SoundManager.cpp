#include "pch.h"
#include "SoundManager.h"
#include <SFML/Audio.hpp>

using namespace sf;

SoundManager::SoundManager()
{
	m_FireBuffer.loadFromFile("sounds/fire1.wav");
	m_FallInFireBuffer.loadFromFile("sounds/fallinfire.wav");
	m_FallInWaterBuffer.loadFromFile("sounds/fallinwater.wav");
	m_JumpBuffer.loadFromFile("sounds/jump.wav");
	m_ReachGoalBuffer.loadFromFile("sounds/reachgoal.wav");

	m_Fire1Sound.setBuffer(m_FireBuffer);
	m_Fire2Sound.setBuffer(m_FireBuffer);
	m_Fire3Sound.setBuffer(m_FireBuffer);
	m_FallInFireSound.setBuffer(m_FallInFireBuffer);
	m_FallInWaterSound.setBuffer(m_FallInWaterBuffer);
	m_JumpSound.setBuffer(m_JumpBuffer);
	m_ReachGoalSound.setBuffer(m_ReachGoalBuffer);

	float minDistance = 150;
	float attenuation = 15;

	m_Fire1Sound.setAttenuation(attenuation);
	m_Fire2Sound.setAttenuation(attenuation);
	m_Fire3Sound.setAttenuation(attenuation);

	m_Fire1Sound.setMinDistance(minDistance);
	m_Fire2Sound.setMinDistance(minDistance);
	m_Fire3Sound.setMinDistance(minDistance);

	m_Fire1Sound.setLoop(1);
	m_Fire2Sound.setLoop(1);
	m_Fire3Sound.setLoop(1);
}

void SoundManager::playFire(Vector2f emitterLocation, Vector2f listenerLocation)
{
	Listener::setPosition(listenerLocation.x, listenerLocation.y, 0.0f);

	switch (m_NextSound)
	{
	case 1:
		m_Fire1Sound.setPosition(emitterLocation.x, emitterLocation.y, 0.0f);
		if (m_Fire1Sound.getStatus() == Sound::Status::Stopped)
		{
			m_Fire1Sound.play();
		}
		break;

	case 2:
		m_Fire2Sound.setPosition(emitterLocation.x, emitterLocation.y, 0.0f);
		if (m_Fire2Sound.getStatus() == Sound::Status::Stopped)
		{
			m_Fire2Sound.play();
		}
		break;

	case 3:
		m_Fire3Sound.setPosition(emitterLocation.x, emitterLocation.y, 0.0f);
		if (m_Fire3Sound.getStatus() == Sound::Status::Stopped)
		{
			m_Fire3Sound.play();
		}
		break;
	}

	m_NextSound = m_NextSound % 3 + 1;
}

void SoundManager::stopPlayFire() 
{
	m_Fire1Sound.stop();
	m_Fire2Sound.stop();
	m_Fire3Sound.stop();
}

void SoundManager::playFallInFire()
{
	m_FallInFireSound.setRelativeToListener(1);
	m_FallInFireSound.play();
}

void SoundManager::playFallInWater()
{
	m_FallInWaterSound.setRelativeToListener(1);
	m_FallInWaterSound.play();
}

void SoundManager::playJump()
{
	m_JumpSound.setRelativeToListener(1);
	m_JumpSound.play();
}

void SoundManager::playReachGoal()
{
	m_ReachGoalSound.setRelativeToListener(1);
	m_ReachGoalSound.play();
}