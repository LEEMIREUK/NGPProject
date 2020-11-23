#include "Bullet.h"

Bullet::Bullet()
{
	shoot = false;
	bulletSpeed = 0;
	bulletX = 0;
	bulletY = 0;
}

Bullet::~Bullet()
{
}

void Bullet::DrawBullet()
{
	glPushMatrix();
	glTranslatef(bulletSpeed, 0, 0);
	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_QUADS);
	glVertex2f(0, 3);
	glVertex2f(0, -3);
	glVertex2f(15, -3);
	glVertex2f(15, 3);
	glEnd();
	glPopMatrix();
}

void Bullet::UpdateSpeed(float speed)
{
	bulletSpeed += speed;
}

bool Bullet::GetShoot()
{
	return shoot;
}

void Bullet::SetShoot(bool shooting)
{
	shoot = shooting;
}

void Bullet::GetPos(float x, float y)
{
	bulletX = x;
	bulletY = y;
}
