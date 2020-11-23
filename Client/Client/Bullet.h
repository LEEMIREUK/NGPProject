#pragma once
#include "Global.h"
#include "Player.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void DrawBullet();
	void UpdateSpeed(float speed);
	bool GetShoot();
	void SetShoot(bool shooting);
	void GetPos(float x, float y);
private:
	float bulletSpeed;
	float bulletX, bulletY;
	bool shoot;
	Player player;
};

