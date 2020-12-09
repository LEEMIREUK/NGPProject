#include "Weapon.h"

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}

void Weapon::DrawWeapon()
{
	glPushMatrix();
	// 颊棱捞-------------------
	glPushMatrix();
	glTranslatef(0, 15, 0);
	glColor3f(0.5f, 0.5f, 0.2f);
	glBegin(GL_QUADS);
	glVertex2f(0, 4);
	glVertex2f(0, -4);
	glVertex2f(8, -4);
	glVertex2f(8, 4);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -15, 0);
	glColor3f(0.5f, 0.5f, 0.2f);
	glBegin(GL_QUADS);
	glVertex2f(0, 4);
	glVertex2f(0, -4);
	glVertex2f(5, -4);
	glVertex2f(5, 4);
	glEnd();
	glPopMatrix();
	//--------------------------

	// 个烹---------------------
	glPushMatrix();
	glTranslatef(5, 0, 0);
	glColor3f(0.1f, 0.3f, 0.2f);
	glBegin(GL_QUADS);
	glVertex2f(0, 23);
	glVertex2f(0, -23);
	glVertex2f(15, -23);
	glVertex2f(15, 23);
	glEnd();
	glPopMatrix();
	//--------------------------

	// 醚备---------------------
	glPushMatrix();
	glTranslatef(20, 0, 0);
	glColor3f(0.6f, 0.4f, 0.2f);
	glBegin(GL_QUADS);
	glVertex2f(0, 10);
	glVertex2f(0, -10);
	glVertex2f(30, -10);
	glVertex2f(30, 10);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(45, 0.f, 0.f);
	glColor3f(0.2f, 0.3f, 0.7f);
	glBegin(GL_QUADS);
	glVertex2f(0, 10);
	glVertex2f(0, -10);
	glVertex2f(3, -10);
	glVertex2f(3, 10);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(40, 0.f, 0.f);
	glColor3f(0.2f, 0.3f, 0.7f);
	glBegin(GL_QUADS);
	glVertex2f(0, 10);
	glVertex2f(0, -10);
	glVertex2f(3, -10);
	glVertex2f(3, 10);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(35, 0.f, 0.f);
	glColor3f(0.2f, 0.3f, 0.7f);
	glBegin(GL_QUADS);
	glVertex2f(0, 10);
	glVertex2f(0, -10);
	glVertex2f(3, -10);
	glVertex2f(3, 10);
	glEnd();
	glPopMatrix();
	//--------------------------

	// 醚备港-------------------
	glPushMatrix();
	glTranslatef(50, 0, 0);
	glColor3f(0.2f, 0.3f, 0.7f);
	glBegin(GL_QUADS);
	glVertex2f(0, 3);
	glVertex2f(0, -3);
	glVertex2f(5, -3);
	glVertex2f(5, 3);
	glEnd();
	glPopMatrix();
	//--------------------------

	glPopMatrix();
}