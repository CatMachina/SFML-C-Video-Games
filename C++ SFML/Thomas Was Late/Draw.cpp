#include "pch.h"
#include "Engine.h"

void Engine::draw()
{
	m_Window.clear(Color::White);

	m_RippleShader.setUniform("uTime", m_GameTimeTotal.asSeconds());

	if (!m_SplitScreen)
	{
		m_Window.setView(m_BGMainView);
//		m_Window.draw(m_BackgroundSprite);
		m_Window.draw(m_BackgroundSprite, &m_RippleShader);
		m_Window.setView(m_MainView);

		m_Window.draw(m_VALevel, &m_TextureTiles);

		m_Window.draw(m_Thomas.getSprite());
		m_Window.draw(m_Bob.getSprite());

		// Particles
		if (m_PS.running())
		{
			m_Window.draw(m_PS);
		}

	} // Split Screen
	else
	{
		// Thomas
		m_Window.setView(m_BGLeftView);
//		m_Window.draw(m_BackgroundSprite);
		m_Window.draw(m_BackgroundSprite, &m_RippleShader);
		m_Window.setView(m_LeftView);

		m_Window.draw(m_VALevel, &m_TextureTiles);

		m_Window.draw(m_Bob.getSprite());
		m_Window.draw(m_Thomas.getSprite());

		if (m_PS.running())
		{
			m_Window.draw(m_PS);
		}

		// Bob
		m_Window.setView(m_BGRightView);
//		m_Window.draw(m_BackgroundSprite);
		m_Window.draw(m_BackgroundSprite, &m_RippleShader);
		m_Window.setView(m_RightView);

		m_Window.draw(m_VALevel, &m_TextureTiles);

		m_Window.draw(m_Thomas.getSprite());
		m_Window.draw(m_Bob.getSprite());

		if (m_PS.running())
		{
			m_Window.draw(m_PS);
		}

	} // Not Split Screen
	m_Window.setView(m_HudView);
	
	// Draw HUD
	m_Window.setView(m_HudView);
	m_Window.draw(m_HUD.getLevel());
	m_Window.draw(m_HUD.getTime());
	if (!m_Playing)
	{
		m_Window.draw(m_HUD.getMessage());
	}
	
	// Final
	m_Window.display();
}