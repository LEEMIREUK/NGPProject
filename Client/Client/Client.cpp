#include <WS2tcpip.h>
#include <iostream>
#pragma comment (lib, "ws2_32")
#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Map.h"
#include "../../protocol.h"

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
char recvBuffer[MAX_BUFFER];
int myID;

SOCKET cSocket;
sockaddr_in sockAddr;

void error_display(const char* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L"에러 (" << err_no << ") " << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}


void process_packet(char* packet)
{
	switch (packet[1])
	{
		case stoc_new_client:
		{
			STOC_NEW_CLIENT* p = reinterpret_cast<STOC_NEW_CLIENT*>(packet);
			myID = p->id;
			player.SetPos(p->x, p->y);
			player.SetSize(p->player_size);
			break;
		}
		case stoc_move:
		{
			std::cout << "recv move packet" << std::endl;
			STOC_MOVE* p = reinterpret_cast<STOC_MOVE*>(packet);
			if (p->id == myID)
			{
				std::cout << p->x << ", " << p->y << std::endl;
				player.SetPos(p->x, p->y);
			}
			break;
		}
		case stoc_world_state:
		{
			STOC_WORLD_STATE* p = reinterpret_cast<STOC_WORLD_STATE*>(packet);
			for (int i = 0; i < 2; ++i)
			{
				if (p->clients_state[i].is_connected && i == myID)
				{
					//player.SetPos(p->clients_state[i].x, p->clients_state[i].y);
				}
			}
			break;
		}
	}
}

void process_recv(int recv_bytes)
{
	char* ptr = recvBuffer;
	static int in_packet_size = 0;
	static int saved_packet_size = 0;
	static char packet_buffer[MAX_BUFFER];

	while (0 != recv_bytes)
	{
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (recv_bytes + saved_packet_size >= in_packet_size)
		{
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			process_packet(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			recv_bytes -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else
		{
			memcpy(packet_buffer + saved_packet_size, ptr, recv_bytes);
			saved_packet_size += recv_bytes;
			recv_bytes = 0;
		}
	}
}

void GetAngle(int mouse_x, int mouse_y)
{
	float width = fabs(mouse_x - player.GetX());
	float height = fabs(mouse_y - player.GetY());
	float radian = atan2(height, width);

	rotate = radian * 180 / PI;
}

void KeyDownInput(unsigned char key, int x, int y)
{
	CTOS_MOVE p;
	p.id = myID;
	p.size = sizeof(p);
	p.type = ctos_move;
	p.time = std::chrono::system_clock::now();

	switch (key)
	{
	case 'w' | 'W':
		inputs.KEY_W = true;
		p.dir = DIR_UP;
		break;
	case 'a' | 'A':
		inputs.KEY_A = true;
		p.dir = DIR_LEFT;
		break;
	case 's' | 'S':
		inputs.KEY_S = true;
		p.dir = DIR_DOWN;
		break;
	case 'd' | 'D':
		inputs.KEY_D = true;
		p.dir = DIR_RIGHT;
		break;
	}

	send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
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

	int recv_result = recv(cSocket, recvBuffer, MAX_BUFFER, 0);
	if (recv_result == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			closesocket(cSocket);
			WSACleanup();
		}
		
	}
	else if (recv_result == 0) return;
	else process_recv(recv_result);

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.f, 1.f, 1.f);

	glPushMatrix();
		glTranslatef(player.GetX(), player.GetY(), 0);
		glRotatef(rotate, 0, 0, 1);
		glTranslatef(-player.GetX(), -player.GetY(), 0);
		glPushMatrix();
			player.DrawPlayer();
			//player.Move(elapsedTimeInSec, &tempInputs);
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
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	cSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(SERVER_PORT);
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int conn_result = connect(cSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

	if (conn_result == 0)
	{
		int recv_bytes = recv(cSocket, recvBuffer, MAX_BUFFER, 0);
		if (recv_bytes > 0) process_recv(recv_bytes);

		unsigned long nonBlocking = 1;
		ioctlsocket(cSocket, FIONBIO, &nonBlocking);

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(WIDTH, HEIGHT);
		glutInitWindowPosition(200, 100);
		glutCreateWindow("Shooting Nemo");
		glutDisplayFunc(display);

		// 키보드 처리
		glutKeyboardFunc(KeyDownInput);
		glutKeyboardUpFunc(KeyUpInput);

		////Init inputs
		//memset(&inputs, 0, sizeof(Inputs));

		// 마우스 처리
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
	else return;
}

int main(int argc, char** argv)
{
	std::wcout.imbue(std::locale("korean"));
	InitOpenGL(argc, argv);
	return 0;
}