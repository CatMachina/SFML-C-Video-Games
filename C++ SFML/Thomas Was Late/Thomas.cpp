#include "pch.h"
#include "Thomas.h"
#include "TextureHolder.h"

Thomas::Thomas()
{
	m_Sprite = Sprite(TextureHolder::getTexture("graphics/thomas.png"));

	m_JumpDuration = 0.45;
}

bool Thomas::handleInput()
{
	m_JustJumped = 0;
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		if (!m_IsJumping && !m_IsFalling)
		{
			m_IsJumping = 1;
			m_TimeThisJump = 0;
			m_JustJumped = 1;
		}

	}
	else 
	{
		m_IsJumping = 0;
		m_IsFalling = 1;
	}
	
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		m_LeftPressed = 1;
	}
	else
	{
		m_LeftPressed = 0;
	}

	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		m_RightPressed = 1;
	}
	else
	{
		m_RightPressed = 0;
	}

	return m_JustJumped;
}