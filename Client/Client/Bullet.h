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
	void SetSizeX(float size) { size_x = size; }
	void SetSizeY(float size) { size_y = size; }
private:
	float bulletSpeed;
	float startX, startY;
	float bulletX, bulletY;
	float size_x, size_y;
	bool shoot;
	float angle;
	float radian;
};

