#pragma once
#include "Global.h"

class Weapon
{
public:
	Weapon();
	~Weapon();

	void DrawWeapon();
private:
	float m_bulletX, m_bulletY;
	float bulletSpeed;
};