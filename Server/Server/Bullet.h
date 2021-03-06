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
	int damage;
	int w_type;
	bool active;
public:
	Bullet();
	Bullet(float x, float y, float ang, float rad, int shooter, int type)
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
		w_type = type;
		if (w_type == 1) damage = 20;
		else damage = 10;
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
	int GetType() { return w_type; }
	int GetDamage() { return damage; }

	void SetStartX(float new_x) { start_x = new_x; }
	void SetStartY(float new_y) { start_y = new_y; }
	void SetAngle(float new_angle) { angle = new_angle; }
	void SetBulletX(float new_x) { bullet_x = new_x; }
	void SetBulletY(float new_y) { bullet_y = new_y; }
	void SetID(float new_id) { id = new_id; }
	void SetRotate(float rad) { radian = rad; }
	void SetActve(bool value) { active = value; }
	void SetType(int type) { w_type = type; }

	void Update(std::chrono::duration<double>& time);
};

