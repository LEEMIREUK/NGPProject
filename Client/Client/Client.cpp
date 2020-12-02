#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "ws2_32")
#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Map.h"
#include "../../protocol.h"

Player player[2];
Weapon weapon;
//Bullet bullet[BULLETCOUNT];
std::vector<Bullet> bullets;
Map map;
Inputs inputs;

int g_prevTimeInMillisecond = 0;
int shootcount = 0;
float rotate = 0;
float radian = 0;
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
	switch (packet[2])
	{
		case stoc_login_ok:
		{
			STOC_LOGIN_OK* p = reinterpret_cast<STOC_LOGIN_OK*>(packet);
			myID = p->id;
			player[myID].SetPos(p->x, p->y);
			player[myID].SetSize(p->player_size);
			player[myID].SetConnected(true);
			break;
		}
		case stoc_new_client:
		{
			STOC_NEW_CLIENT* p = reinterpret_cast<STOC_NEW_CLIENT*>(packet);
			player[p->id].SetPos(p->x, p->y);
			player[p->id].SetSize(p->player_size);
			player[p->id].SetConnected(true);
			break;
		}
		case stoc_move:
		{
			STOC_MOVE* p = reinterpret_cast<STOC_MOVE*>(packet);
			player[p->id].SetPos(p->x, p->y);
			break;
		}
		case stoc_world_state:
		{
			STOC_WORLD_STATE* p = reinterpret_cast<STOC_WORLD_STATE*>(packet);
			bullets.resize(p->b_num);
			for (int i = 0; i < p->b_num; ++i)
			{
				bullets[i].SetPos(p->bullets_state[i].x, p->bullets_state[i].y);
				bullets[i].SetShootAngle(p->bullets_state[i].angle);
			}
			//for (int i = 0; i < 2; ++i)
			//{
			//	if (p->clients_state[i].is_connected && i == myID)
			//	{
			//		player.SetPos(p->clients_state[i].x, p->clients_state[i].y);
			//	}
			//}
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
		if (0 == in_packet_size) in_packet_size = *(reinterpret_cast<short*>(&ptr[0]));
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
		CTOS_SHOOT p;
		p.x = player[myID].GetX();
		p.y = player[myID].GetY();
		p.radian = radian;
		p.angle = rotate;
		p.id = myID;
		p.size = sizeof(p);
		p.type = ctos_shoot;
		send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
		//// 총알을 쏠때 초기 위치를 잡아줄 플레이어의 좌표 받기
		//bullet[shootcount].SetPos(player[myID].GetX(), player[myID].GetY());
		//bullet[shootcount].SetShootAngle(rotate);
		//bullet[shootcount].SetRadian(radian);
		//bullet[shootcount].SetShoot(true);

		//if (shootcount > BULLETCOUNT - 1)
		//{
		//	shootcount = -1;
		//}
		//shootcount += 1;
	}
}

void ProcessMouseMotion(int x, int y)
{
	float mx = x;
	float my = HEIGHT - y;
	float width = fabs(mx - player[myID].GetX());
	float height = fabs(my - player[myID].GetY());
	float radian;

	if (player[myID].GetX() < mx)
	{
		if (player[myID].GetY() < my)
			radian = atan2(height, width);
		else
			radian = atan2(-height, width);

	}
	else
	{
		if (player[myID].GetY() < my)
			radian = atan2(height, -width);
		else
			radian = atan2(-height, -width);

	}
	rotate = radian * 180 / PI;
}

void display()
{ 
	std::cout << radian << std::endl;
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

	for(auto& p : player)
	{
		if (p.GetConnected())
		{
			glPushMatrix();
			glTranslatef(p.GetX(), p.GetY(), 0);
			glRotatef(rotate, 0, 0, 1);
			glTranslatef(-p.GetX(), -p.GetY(), 0);
			glPushMatrix();
			p.DrawPlayer();
			weapon.DrawWeapon();
			glPopMatrix();
			glPopMatrix();
		}
	}

	glPushMatrix();
		//for (int i = 0; i < BULLETCOUNT; ++i)
		//{
		//	if (bullet[i].GetShoot())
		//	{
		//		bullet[i].UpdateSpeed(BULLETSPEED * elapsedTimeInSec);
		//		bullet[i].DrawBullet();
		//	}
		//}
		for (auto& b : bullets)
		{
			b.UpdateSpeed(BULLETSPEED * elapsedTimeInSec);
			b.DrawBullet();
		}
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
	bullets.reserve(100);
	InitOpenGL(argc, argv);
	closesocket(cSocket);
	WSACleanup();
	return 0;
}