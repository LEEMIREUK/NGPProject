#pragma once
#include "GL/glut.h"
#include <iostream>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
#define MOVESPEED 300
#define BULLETSPEED 1
#define BULLETCOUNT 30
#define PI 3.1415926535

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