#include "pch.h"
#include "Engine.h"

void Engine::input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::KeyPressed)
		{
			// Escape
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				m_Window.close();
			}
			
			// Enter
			if (Keyboard::isKeyPressed(Keyboard::Return))
			{
				m_Playing = 1;
			}

			// Q
			if (Keyboard::isKeyPressed(Keyboard::Q))
			{
				m_Character1 = !m_Character1;
			}

			// E
			if (Keyboard::isKeyPressed(Keyboard::E))
			{
				m_SplitScreen = !m_SplitScreen;
			}
		}
	} // End Event Polling

	// Thomas
	if (m_Thomas.handleInput())
	{
		m_SM.playJump();
	}

	// Bob
	if (m_Bob.handleInput())
	{
		m_SM.playJump();
	}

}