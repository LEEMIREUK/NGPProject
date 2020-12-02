#pragma once
#include "Global.h"
#include "Player.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void InitBullet();
	void DrawBullet();
	void UpdateSpeed(float speed);
	bool GetShoot();
	void SetShoot(bool shooting);
	void SetPos(float x, float y);
	void SetShootAngle(float rotateAngle);
	void SetRadian(float rotateradian);
	void SetShootSpeed(float speed) { bulletSpeed = speed; }
private:
	float bulletSpeed;
	float startX, startY;
	float bulletX, bulletY;
	bool shoot;
	float angle;
	float radian;
	Player player;
};

