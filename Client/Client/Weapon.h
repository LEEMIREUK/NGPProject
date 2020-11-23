#pragma once
#include "Global.h"

class Weapon
{
public:
	Weapon();
	~Weapon();

	void DrawWeapon();
	void DrawBullet();
private:
	float m_bulletX, m_bulletY;
	float bulletSpeed;
};