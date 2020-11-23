#include <WinSock2.h>
#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Map.h"

Player player;
Weapon weapon;
Bullet bullet[BULLETCOUNT];
Map map;
Inputs inputs;
int g_prevTimeInMillisecond = 0;
bool clicked = false;
int shootcount = 0;
float rotate = 0;
float rotate1 = 0;
float player_angle = 0;

void GetAngle(int mouse_x, int mouse_y)
{
	float width = fabs(mouse_x - player.GetX());
	float height = fabs(mouse_y - player.GetY());
	float radian = atan2(height, width);

	rotate = radian * 180 / PI;
}

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
	}
}

void ProcessMouseMotion(int x, int y)
{
	float mx = x;
	float my = HEIGHT - y;
	float width = fabs(mx - player.GetX());
	float height = fabs(my - player.GetY());
	float radian;

	if (player.GetX() < mx)
	{
		if (player.GetY() < my)
			radian = atan2(height, width);
		else
			radian = atan2(-height, width);

	}
	else
	{
		if (player.GetY() < my)
			radian = atan2(height, -width);
		else
			radian = atan2(-height, -width);

	}
	rotate = radian * 180 / PI;
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
		glTranslatef(player.GetX(), player.GetY(), 0);
		glRotatef(rotate, 0, 0, 1);
		glTranslatef(-player.GetX(), -player.GetY(), 0);
		glPushMatrix();
			player.DrawPlayer();
			player.Move(elapsedTimeInSec, &tempInputs);
			weapon.DrawWeapon();

			glPushMatrix();
				for (int i = 0; i < BULLETCOUNT; ++i)
				{
					if (bullet[i].GetShoot())
					{
						glLoadIdentity();
						glTranslatef(player.GetX() + 70, player.GetY(), 0);
						bullet[i].UpdateSpeed(BULLETSPEED);
						bullet[i].DrawBullet();
					}
				}
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	map.DrawMap();
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
	//gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);
	gluOrtho2D(0, WIDTH, 0, HEIGHT);

	glutMainLoop();
}

int main(int argc, char** argv)
{
	//WSADATA WSAData;
	//WSAStartup(MAKEWORD(2, 2), &WSAData);

	InitOpenGL(argc, argv);
	return 0;
}