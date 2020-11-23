#include <WinSock2.h>
#include <iostream>
#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"

Player player;
Weapon weapon;
Bullet bullet[BULLETCOUNT];
Inputs inputs;
int g_prevTimeInMillisecond = 0;
bool clicked = false;
int shootcount = 0;

void KeyDownInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w' | 'W':
		inputs.KEY_W = true;
		break;
	case 'a' | 'A':
		inputs.KEY_A = true;
		break;
	case 's' | 'S':
		inputs.KEY_S = true;
		break;
	case 'd' | 'D':
		inputs.KEY_D = true;
		break;
	}
}

void KeyUpInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w' | 'W':
		inputs.KEY_W = false;
		break;
	case 'a' | 'A':
		inputs.KEY_A = false;
		break;
	case 's' | 'S':
		inputs.KEY_S = false;
		break;
	case 'd' | 'D':
		inputs.KEY_D = false;
		break;
	}
}

void ProcessMouse(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if (shootcount > BULLETCOUNT - 1)
		{
			shootcount = 0;
		}
		bullet[shootcount].SetShoot(true);
		shootcount += 1;
		clicked = true;
	}
	if (clicked)
	{
		//if (shootcount > BULLETCOUNT - 1)
		//	shootcount = 0;
		//bullet[shootcount].SetShoot(true);
		//shootcount += 1;
		clicked = false;
	}
}

void ProcessMouseMotion(int x, int y)
{
}

void display()
{ 
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currentTime - g_prevTimeInMillisecond;
	g_prevTimeInMillisecond = currentTime;
	float elapsedTimeInSec = (float)elapsedTime / 1000.f;

	Inputs tempInputs;
	memcpy(&tempInputs, &inputs, sizeof(Inputs));

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.f, 1.f, 1.f);

	glPushMatrix();
	player.DrawPlayer();
	player.Move(elapsedTimeInSec, &tempInputs);
	weapon.DrawWeapon();
	glPopMatrix();

	glPushMatrix();
	for (int i = 0; i < BULLETCOUNT; ++i)
	{
		if (bullet[i].GetShoot())
		{
			bullet[i].GetPos(player.GetX(), player.GetY());
			bullet[i].UpdateSpeed(BULLETSPEED);
			bullet[i].DrawBullet();
		}
	}
	glPopMatrix();

	glFlush();
}

void Timerfunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1, Timerfunction, 1);
}

void InitOpenGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Shooting Nemo");
	glutDisplayFunc(display);

	// 虐焊靛 贸府
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);

	////Init inputs
	//memset(&inputs, 0, sizeof(Inputs));

	// 付快胶 贸府
	glutMouseFunc(ProcessMouse);
	glutPassiveMotionFunc(ProcessMouseMotion);

	g_prevTimeInMillisecond = glutGet(GLUT_ELAPSED_TIME);
	
	glutTimerFunc(1, Timerfunction, 1);
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);

	glutMainLoop();
}

int main(int argc, char** argv)
{
	//WSADATA WSAData;
	//WSAStartup(MAKEWORD(2, 2), &WSAData);


	InitOpenGL(argc, argv);
	return 0;
}