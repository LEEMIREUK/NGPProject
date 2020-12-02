#include "Player.h"

Player::Player()
{
	m_PositionX = 600;
	m_PositionY = 400;
	m_Speed_X = 0;
	m_Speed_Y = 0;
	m_Size = 30;
	m_PlayerID = NULL;
	m_connected = false;
}

Player::~Player()
{
}

void Player::DrawPlayer()
{
	glTranslatef(m_PositionX, m_PositionY, 0.f);
	glPushMatrix(); 
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.4f, 0.2f);
	glVertex2f(-m_Size, -m_Size);
	glVertex2f(-m_Size, +m_Size);
	glVertex2f(0, +m_Size);
	glVertex2f(0, -m_Size);
	glEnd();
	glPopMatrix();
}

void Player::Move(float elapsedTimeInSec, Inputs* inputs)
{
	float t = elapsedTimeInSec;
	if (inputs->KEY_W)
	{
		m_PositionY += MOVESPEED * t;
	}
	if (inputs->KEY_A)
	{
		m_PositionX -= MOVESPEED * t;
	}
	if (inputs->KEY_S)
	{
		m_PositionY -= MOVESPEED * t;
	}
	if (inputs->KEY_D)
	{
		m_PositionX += MOVESPEED * t;
	}
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

void Player::SetConnected(bool value)
{
	m_connected = value;
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

bool Player::GetConnected()
{
	return m_connected;
}

float Player::GetX()
{
	return m_PositionX;
}

float Player::GetY()
{
	return m_PositionY;
}
