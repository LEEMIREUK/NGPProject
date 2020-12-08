#pragma once
#include <chrono>

class Bullet
{
	float start_x, start_y;
	float angle;
	float radian;
	float bullet_x, bullet_y;
	float speed;
	int id;
	bool active;
public:
	Bullet();
	Bullet(float x, float y, float ang, float rad, int shooter)
	{
		start_x = x;
		start_y = y;
		angle = ang;
		radian = rad;
		id = shooter;
		bullet_x = 0.f;
		bullet_y = 0.f;
		speed = 0;
		active = true;
	}
	~Bullet() {}

	float GetStartX() { return start_x; }
	float GetStartY() { return start_y; }
	float GetAngle() { return angle; }
	float GetBulletX() { return bullet_x; }
	float GetBulletY() { return bullet_y; }
	float GetSpeed() { return speed; }
	float GetRadian() { return radian; }
	int GetID() { return id; }
	bool GetActive() { return active; }

	void SetStartX(float new_x) { start_x = new_x; }
	void SetStartY(float new_y) { start_y = new_y; }
	void SetAngle(float new_angle) { angle = new_angle; }
	void SetBulletX(float new_x) { bullet_x = new_x; }
	void SetBulletY(float new_y) { bullet_y = new_y; }
	void SetID(float new_id) { id = new_id; }
	void SetRotate(float rad) { radian = rad; }
	void SetActve(bool value) { active = value; }

	void Update(std::chrono::duration<double>& time);
};

