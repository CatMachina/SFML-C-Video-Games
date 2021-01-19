#include "pch.h"
#include "Engine.h"

void Engine::loadLevel()
{
	m_Playing = 0;

	for (int i = 0; i < m_LM.getLevelSize().y; i++)
	{
		delete[] m_ArrayLevel[i];
	}
	delete[] m_ArrayLevel;

	m_ArrayLevel = m_LM.nextLevel(m_VALevel);
	populateEmitters(m_FireEmitters, m_ArrayLevel);
	m_TimeRemaining = m_LM.getTimeLimit();

	m_Thomas.spawn(m_LM.getStartPosition(), GRAVITY);
	m_Bob.spawn(m_LM.getStartPosition(), GRAVITY);

	m_NewLevelRequired = 0;
}