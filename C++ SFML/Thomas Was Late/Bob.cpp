#include "pch.h"
#include "Bob.h"
#include "TextureHolder.h"

Bob::Bob()
{
	m_Sprite = Sprite(TextureHolder::getTexture("graphics/bob.png"));
	m_JumpDuration = 0.25;
}

bool Bob::handleInput()
{
	m_JustJumped = 0;
	if (Keyboard::isKeyPressed(Keyboard::Up))
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

	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		m_LeftPressed = 1;
	}
	else
	{
		m_LeftPressed = 0;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		m_RightPressed = 1;
	}
	else
	{
		m_RightPressed = 0;
	}

	return m_JustJumped;
}