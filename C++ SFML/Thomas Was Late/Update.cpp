#include "pch.h"
#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

void Engine::update(float dtAsSeconds)
{

	if (m_NewLevelRequired)
	{
		loadLevel();
	}

	if (m_Playing)
	{
		// Update Thomas and Bob
		m_Thomas.update(dtAsSeconds);
		m_Bob.update(dtAsSeconds);

		if (detectCollisions(m_Thomas) & detectCollisions(m_Bob))
		{
			m_NewLevelRequired = 1;
			// Goal sound
			m_SM.playReachGoal();
		}

		if (m_Bob.getFeet().intersects(m_Thomas.getHead()))
		{
			m_Bob.stopFalling(m_Thomas.getHead().top);
		}
		else if (m_Thomas.getFeet().intersects(m_Bob.getHead()))
		{
			m_Thomas.stopFalling(m_Bob.getHead().top);
		}

		m_TimeRemaining -= dtAsSeconds;
		if (m_TimeRemaining <= 0)
		{
			m_NewLevelRequired = 1;
		}
	} // End Playing

	vector<Vector2f>::iterator it;
	bool stop = 1;
	for (it = m_FireEmitters.begin(); it != m_FireEmitters.end(); it++)
	{
		float posX = (*it).x;
		float posY = (*it).y;

		FloatRect localRect(posX - 250, posY - 250, 500, 500);
		if (m_Thomas.getPosition().intersects(localRect))
		{
			m_SM.playFire(Vector2f(posX, posY), m_Thomas.getCenter());
			stop = 0;
		}
	}
	if (stop)
	{
		m_SM.stopPlayFire();
	}

	if (m_SplitScreen)
	{
		m_LeftView.setCenter(m_Thomas.getCenter());
		m_RightView.setCenter(m_Bob.getCenter());
	}
	else
	{
		if (m_Character1)
		{
			m_MainView.setCenter(m_Thomas.getCenter());
		}
		else
		{
			m_MainView.setCenter(m_Bob.getCenter());
		}
	}

	m_FramesSinceLastHUDUpdate++;
	if (m_FramesSinceLastHUDUpdate > m_TargetFramesPerHUDUpdate)
	{
		stringstream ssTime;
		stringstream ssLevel;

		ssTime << (int)m_TimeRemaining;
		m_HUD.setTime(ssTime.str());

		ssLevel << "Level: " << m_LM.getCurrentLevel();
		m_HUD.setLevel(ssLevel.str());
		
		m_FramesSinceLastHUDUpdate = 0;
	}

	if (m_PS.running())
	{
		m_PS.update(dtAsSeconds);
	}
}