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
	glTranslatef(bulletX, bulletY, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(bulletSpeed + 70, 0, 0);
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

void Bullet::SetPos(float playerX, float playerY)
{
	bulletX = playerX;
	bulletY = playerY;
}

void Bullet::SetShootAngle(float rotateAngle)
{
	angle = rotateAngle;
}
