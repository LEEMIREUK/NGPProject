#include "Bullet.h"
#include "../../protocol.h"

Bullet::Bullet()
{
	InitBullet();
}

Bullet::~Bullet()
{
}

void Bullet::InitBullet()
{
	shoot = false;
	bulletSpeed = 0;
	bulletX = 0;
	bulletY = 0;
	startX = 0;
	startY = 0;
}

void Bullet::DrawBullet()
{
	glPushMatrix();
	glTranslatef(startX, startY, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(BULLETSPEED + 50, 0, 0);
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
	bulletX = startX + (bulletSpeed + 50) * cos(radian);
	bulletY = startY + bulletSpeed * sin(radian);
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
	startX = playerX;
	startY = playerY;
}

void Bullet::SetShootAngle(float rotateAngle)
{
	angle = rotateAngle;
}

void Bullet::SetRadian(float rotateradian)
{
	radian = rotateradian;
}
