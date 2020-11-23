#include "Map.h"

void Map::DrawMap()
{
	glPushMatrix();
	glLineWidth(10);
	glColor3f(0.f, 0.f, 1.f);

	//glBegin(GL_LINES);
	//glVertex2f(-WIDTH / 2, HEIGHT / 2);
	//glVertex2f(-WIDTH / 2, -HEIGHT / 2);

	//glVertex2f(-WIDTH / 2, -HEIGHT / 2);
	//glVertex2f(WIDTH / 2, -HEIGHT / 2);

	//glVertex2f(WIDTH / 2, HEIGHT / 2);
	//glVertex2f(WIDTH / 2, -HEIGHT / 2);

	//glVertex2f(-WIDTH / 2, HEIGHT / 2);
	//glVertex2f(WIDTH / 2, HEIGHT / 2);
	//glEnd();


	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, HEIGHT);

	glVertex2f(0, 0);
	glVertex2f(WIDTH, 0);

	glVertex2f(WIDTH, 0);
	glVertex2f(WIDTH, HEIGHT);

	glVertex2f(0, HEIGHT);
	glVertex2f(WIDTH, HEIGHT);
	glEnd();


	glPopMatrix();
}