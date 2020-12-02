#pragma once
#include "Global.h"

class Player
{
public:
	Player();
	~Player();

	void DrawPlayer();
	void Move(float elapsedTimeInSec, Inputs* inputs);
	void Show();
	void SetPos(float x, float y);
	void SetSize(float size);
	void SetConnected(bool value);
	void GetPos(float* x, float* y);
	void GetSize(float* size);
	bool GetConnected();
	float GetX();
	float GetY();
private:
	float m_PositionX, m_PositionY;
	float m_Size;
	float m_Speed_Up;
	float m_Speed_Left;
	float m_Speed_Down;
	float m_Speed_Right;
	float m_Speed_X;
	float m_Speed_Y;
	int	  m_PlayerID;
	bool  m_connected;
};
