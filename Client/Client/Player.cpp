#include "Player.h"

Player::Player()
{
	m_PositionX = 0;
	m_PositionY = 0;
	m_Speed_Up = 0;
	m_Speed_Left = 0;
	m_Speed_Down = 0;
	m_Speed_Right = 0;
	m_Size = 25;
	m_PlayerID = NULL;
}

Player::~Player()
{
}

void Player::DrawPlayer()
{
	glTranslatef(m_Speed_Left + m_Speed_Right, m_Speed_Up + m_Speed_Down, 0.f);
	m_PositionX = m_Speed_Left + m_Speed_Right;
	m_PositionY = m_Speed_Up + m_Speed_Down;

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.4f, 0.2f);
	glVertex2f(-m_Size, -m_Size);
	glVertex2f(-m_Size, +m_Size);
	glVertex2f(+m_Size, +m_Size);
	glVertex2f(+m_Size, -m_Size);
	glEnd();
	glPopMatrix();
}

void Player::Move(float elapsedTimeInSec, Inputs* inputs)
{
	float t = elapsedTimeInSec;

	if (inputs->KEY_W)
	{
		m_Speed_Up += MOVESPEED * t;
	}
	if (inputs->KEY_A)
	{
		m_Speed_Left -= MOVESPEED * t;
	}
	if (inputs->KEY_S)
	{
		m_Speed_Down -= MOVESPEED * t;
	}
	if (inputs->KEY_D)
	{
		m_Speed_Right += MOVESPEED * t;
	}
	if (!inputs->KEY_W)
	{
		m_Speed_Up -= MOVESPEED * t;
		if (m_Speed_Up < 0)
		{
			m_Speed_Up = 0;
		}
	}
	if (!inputs->KEY_A)
	{
		m_Speed_Left += MOVESPEED * t;
		if (m_Speed_Left < 0)
		{
			m_Speed_Left = 0;
		}
	}
	if (!inputs->KEY_S)
	{
		m_Speed_Down += MOVESPEED * t;
		if (m_Speed_Down < 0)
		{
			m_Speed_Down = 0;
		}

	}
	if (!inputs->KEY_D)
	{
		m_Speed_Right -= MOVESPEED * t;
		if (m_Speed_Right < 0)
		{
			m_Speed_Right = 0;
		}
	}
}

void Player::Fire()
{

}

void Player::Show()
{
}

void Player::SetPos(float x, float y)
{
	m_PositionX = x;
	m_PositionY = y;
}

void Player::SetSize(float size)
{
	m_Size = size;
}

void Player::GetPos(float* x, float* y)
{
	*x = m_PositionX;
	*y = m_PositionY;
}
void Player::GetSize(float* size)
{
	*size = m_Size;
}