#include <iostream>
#include "Player.h"

Player player;
Inputs inputs;
int g_prevTimeInMillisecond = 0;

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
	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		player.Fire();
	}
}

void ProcessMouseMotion(int x, int y)
{

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.f, 1.f, 1.f);

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currentTime - g_prevTimeInMillisecond;
	g_prevTimeInMillisecond = currentTime;
	float elapsedTimeInSec = (float)elapsedTime / 1000.f;

	Inputs tempInputs;
	memcpy(&tempInputs, &inputs, sizeof(Inputs));

	player.DrawPlayer();
	player.Move(elapsedTimeInSec, &tempInputs);

	glutSwapBuffers();
}

void Timerfunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(10, Timerfunction, 10);
}

int main(int argc, char** argv)
{
	//WSADATA WSAData;
	//WSAStartup(MAKEWORD(2, 2), &WSAData);

	// 초기화 함수들
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Shooting Nemo");

	//Init inputs
	memset(&inputs, 0, sizeof(Inputs));

	glutDisplayFunc(display);

	// 키보드 처리
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);

	// 마우스 처리
	glutMouseFunc(ProcessMouse);
	glutMotionFunc(ProcessMouseMotion);

	g_prevTimeInMillisecond = glutGet(GLUT_ELAPSED_TIME);

	glutTimerFunc(10, Timerfunction, 10);

	glutMainLoop();

	return 0;
}