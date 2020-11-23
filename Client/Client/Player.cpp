#include "Player.h"

Player::Player()
{
	m_PositionX = 0;
	m_PositionY = 0;
	m_Speed_Up = 0;
	m_Speed_Left = 0;
	m_Speed_Down = 0;
	m_Speed_Right = 0;
	m_Speed_X = 0;
	m_Speed_Y = 0;
	m_Size = 25;
	m_PlayerID = NULL;
}

Player::~Player()
{
}

void Player::DrawPlayer()
{
	glTranslatef(m_Speed_X, m_Speed_Y, 0.f);
	m_PositionX = m_Speed_X;
	m_PositionY = m_Speed_Y;

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
		m_Speed_Y += MOVESPEED;
	}
	if (inputs->KEY_A)
	{
		m_Speed_X -= MOVESPEED;
	}
	if (inputs->KEY_S)
	{
		m_Speed_Y -= MOVESPEED;
	}
	if (inputs->KEY_D)
	{
		m_Speed_X += MOVESPEED;
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

void Player::GetPos(float* x, float* y)
{
	*x = m_PositionX;
	*y = m_PositionY;
}

void Player::GetSize(float* size)
{
	*size = m_Size;
}

int Player::GetX()
{
	return m_PositionX;
}

int Player::GetY()
{
	return m_PositionY;
}
