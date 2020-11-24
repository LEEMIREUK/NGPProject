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
	void SetPos(float x, float y);
	void SetShootAngle(float angle);
private:
	float bulletSpeed;
	float bulletX, bulletY;
	bool shoot;
	float angle;
	Player player;
};

