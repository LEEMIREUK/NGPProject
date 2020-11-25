#include "Map.h"

void Map::DrawMap()
{
	glPushMatrix();
	glLineWidth(10);
	glColor3f(0.f, 0.f, 1.f);

	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 0);
	glVertex2f(WIDTH, 0);
	glVertex2f(WIDTH, HEIGHT);
	glVertex2f(0, HEIGHT);
	glEnd();
	
	glPopMatrix();
}