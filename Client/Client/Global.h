#pragma once
#include "GL/glut.h"

#define WIDTH 1200
#define HEIGHT 800
#define MOVESPEED 0.3
#define BULLETSPEED 1
#define BULLETCOUNT 5

typedef struct Inputs
{
	bool KEY_W;
	bool KEY_A;
	bool KEY_S;
	bool KEY_D;

	bool ARROW_UP;
	bool ARROW_DOWN;
	bool ARROW_LEFT;
	bool ARROW_RIGHT;
};