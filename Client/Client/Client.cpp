#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "ws2_32")
#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Map.h"
#include "../../protocol.h"

std::string ip;
Player player[2];

Weapon weapon;
std::vector<Bullet> bullets;
Map map;
Inputs inputs{ false, false ,false ,false ,false ,false ,false ,false };

int g_prevTimeInMillisecond = 0;
int shootcount = 0;
float radian = 0;
char recvBuffer[MAX_BUFFER];
int myID;
int myWeapon;
bool show_winner = false;
bool get_ready = false;
bool game_start = false;
int winner_id;

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
		case stoc_world_state:
		{
			STOC_WORLD_STATE* p = reinterpret_cast<STOC_WORLD_STATE*>(packet);
			bullets.resize(p->b_num);
			for (int i = 0; i < p->b_num; ++i)
			{
				bullets[i].SetPos(p->bullets_state[i].x, p->bullets_state[i].y);
				bullets[i].SetShootAngle(p->bullets_state[i].angle);
				bullets[i].SetShootSpeed(p->bullets_state[i].speed);
				bullets[i].SetSizeX(p->bullets_state[i].size_x);
				bullets[i].SetSizeY(p->bullets_state[i].size_y);
			}
			for (int i = 0; i < 2; ++i)
			{
				if (p->clients_state[i].is_connected)
				{
					player[i].SetPos(p->clients_state[i].x, p->clients_state[i].y);
					player[i].SetRotate(p->clients_state[i].rotate);
				}
			}
			break;
		}

		case stoc_gameend:
		{
			STOC_GAMEEND* p = reinterpret_cast<STOC_GAMEEND*>(packet);
			show_winner = true;
			game_start = false;
			winner_id = p->winner_id;
			bullets.clear();
			break;
		}

		case stoc_gamestart:
		{
			game_start = true;
			get_ready = false;
			show_winner = false;
			inputs.KEY_A = false;
			inputs.KEY_S = false;
			inputs.KEY_D = false;
			inputs.KEY_W = false;
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

	switch (key)
	{
	case 'w' | 'W':
		if (!inputs.KEY_W && game_start)
		{
			inputs.KEY_W = true;
			p.dir = DIR_UP;
			send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
		}
		break;
	case 'a' | 'A':
		if (!inputs.KEY_A && game_start)
		{
			inputs.KEY_A = true;
			p.dir = DIR_LEFT;
			send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
		}
		break;
	case 's' | 'S':
		if (!inputs.KEY_S && game_start)
		{
			inputs.KEY_S = true;
			p.dir = DIR_DOWN;
			send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
		}
		break;
	case 'd' | 'D':
		if (!inputs.KEY_D && game_start)
		{
			inputs.KEY_D = true;
			p.dir = DIR_RIGHT;
			send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
		}
		break;
	case 'r' | 'R':
		if(!game_start && !get_ready)
		{
			CTOS_READY r;
			r.id = myID;
			r.size = sizeof(r);
			r.type = ctos_ready;
			get_ready = true;
			send(cSocket, reinterpret_cast<char*>(&r), r.size, 0);
			break;
		}
	}

	
}

void KeyUpInput(unsigned char key, int x, int y)
{
	CTOS_MOVE p;
	p.id = myID;
	p.size = sizeof(p);
	p.type = ctos_move;

	switch (key)
	{
	case 'w' | 'W':
		if (inputs.KEY_W && game_start)
		{
			inputs.KEY_W = false;
			p.dir = DIR_UP_UP;
		}
		break;
	case 'a' | 'A':
		if (inputs.KEY_A && game_start)
		{
			inputs.KEY_A = false;
			p.dir = DIR_LEFT_UP;
		}
		break;
	case 's' | 'S':
		if (inputs.KEY_S && game_start)
		{
			inputs.KEY_S = false;
			p.dir = DIR_DOWN_UP;
		}
		break;
	case 'd' | 'D':
		if (inputs.KEY_D && game_start)
		{
			inputs.KEY_D = false;
			p.dir = DIR_RIGHT_UP;
		}
		break;
	}

	send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
}

void ProcessMouse(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && game_start)
	{
		CTOS_SHOOT p;
		p.x = player[myID].GetX();
		p.y = player[myID].GetY();
		p.radian = radian;
		p.angle = player[myID].GetRotate();
		p.id = myID;
		p.w_type = myWeapon;
		p.size = sizeof(p);
		p.type = ctos_shoot;
		send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
	}
}

void ProcessMouseMotion(int x, int y)
{
	if (game_start)
	{
		float mx = x;
		float my = HEIGHT - y;
		float width = fabs(mx - player[myID].GetX());
		float height = fabs(my - player[myID].GetY());

		CTOS_ROTATE p;
		p.id = myID;
		p.size = sizeof(p);
		p.type = ctos_rotate;

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
		player[myID].SetRotate(radian * 180 / PI);

		p.rotate = player[myID].GetRotate();
		send(cSocket, reinterpret_cast<char*>(&p), p.size, 0);
	}
}

void Ready()
{
	float x = 600;
	float y = 400;
	glColor3f(0.6f, 0.8f, 0.5f);
	// R
	glRectf(x - 260, y + 100, x - 240, y - 10);
	glRectf(x - 240, y + 100, x - 200, y + 80);
	glRectf(x - 200, y + 90, x - 180, y + 50);
	glRectf(x - 240, y + 60, x - 200, y + 40);

	glRectf(x - 240, y + 50, x - 220, y + 30);
	glRectf(x - 220, y + 30, x - 200, y + 10);
	glRectf(x - 200, y + 10, x - 180, y - 10);

	// E
	glRectf(x - 150, y + 100, x - 130, y - 10);
	glRectf(x - 130, y + 100, x - 80, y + 80);
	glRectf(x - 130, y + 55, x - 80, y + 35);
	glRectf(x - 130, y + 10, x - 80, y - 10);

	// A
	glRectf(x - 10, y + 100, x + 10, y + 80);
	glRectf(x - 20, y + 80, x, y + 60);
	glRectf(x - 30, y + 60, x - 10, y + 40);
	glRectf(x - 40, y + 40, x - 20, y + 20);
	glRectf(x - 50, y + 20, x - 30, y - 10);
	glRectf(x + 20, y + 80, x, y + 60);
	glRectf(x + 30, y + 60, x + 10, y + 40);
	glRectf(x + 40, y + 40, x + 20, y + 20);
	glRectf(x + 50, y + 20, x + 30, y - 10);
	glRectf(x - 30, y + 40, x + 30, y + 20);

	// D
	glRectf(x + 80, y + 100, x + 100, y - 10);
	glRectf(x + 100, y + 100, x + 120, y + 80);
	glRectf(x + 120, y + 90, x + 140, y + 70);
	glRectf(x + 130, y + 70, x + 150, y + 20);
	glRectf(x + 100, y - 10, x + 120, y + 10);
	glRectf(x + 120, y, x + 140, y + 20);

	// Y
	glRectf(x + 170, y + 100, x + 190, y + 80);
	glRectf(x + 180, y + 90, x + 200, y + 70);
	glRectf(x + 190, y + 80, x + 210, y + 60);
	glRectf(x + 200, y + 70, x + 220, y + 50);
	glRectf(x + 250, y + 100, x + 270, y + 80);
	glRectf(x + 240, y + 90, x + 260, y + 70);
	glRectf(x + 230, y + 80, x + 250, y + 60);
	glRectf(x + 220, y + 70, x + 240, y + 50);
	glRectf(x + 210, y + 60, x + 230, y - 10);

}

void Win()
{
	float x = 620;
	float y = 400;
	glColor3f(0.1f, 0.1f, 0.9f);
	// W
	glRectf(x - 80, y + 100, x - 60, y + 70);
	glRectf(x - 90, y + 70, x - 70, y + 40);
	glRectf(x - 100, y + 40, x - 80, y + 10);
	glRectf(x - 110, y + 10, x - 90, y - 10);
	glRectf(x - 120, y - 10, x - 100, y + 10);
	glRectf(x - 130, y + 10, x - 110, y + 40);
	glRectf(x - 140, y + 70, x - 120, y + 40);
	glRectf(x - 150, y + 100, x - 130, y + 70);

	glRectf(x - 160, y + 100, x - 140, y + 70);
	glRectf(x - 170, y + 70, x - 150, y + 40);
	glRectf(x - 180, y + 40, x - 160, y + 10);
	glRectf(x - 190, y + 10, x - 170, y - 10);
	glRectf(x - 200, y - 10, x - 180, y + 10);
	glRectf(x - 210, y + 10, x - 190, y + 40);
	glRectf(x - 220, y + 70, x - 200, y + 40);
	glRectf(x - 230, y + 100, x - 210, y + 70);

	// I
	glRectf(x - 30, y + 100, x + 30, y + 80);
	glRectf(x - 10, y + 100, x + 10, y);
	glRectf(x - 30, y + 10, x + 30, y - 10);

	// N
	glRectf(x + 60, y + 100, x + 80, y - 10);
	glRectf(x + 60, y + 100, x + 80, y + 80);
	glRectf(x + 70, y + 90, x + 90, y + 70);
	glRectf(x + 80, y + 80, x + 100, y + 60);
	glRectf(x + 90, y + 70, x + 110, y + 50);
	glRectf(x + 100, y + 60, x + 120, y + 40);
	glRectf(x + 110, y + 50, x + 130, y + 30);
	glRectf(x + 120, y + 40, x + 140, y + 20);
	glRectf(x + 130, y + 30, x + 150, y + 10);
	glRectf(x + 140, y + 20, x + 160, y);
	glRectf(x + 150, y + 100, x + 170, y - 10);
}

void Lose()
{
	float x = 380;
	float y = 500;

	// L
	glColor3f(0.9f, 0.1f, 0.1f);
	glRectf(x + 20, y, x + 40, y - 110);
	glRectf(x + 20, y - 90, x + 100, y - 110);

	// O
	glRectf(x + 120, y - 20, x + 140, y - 90);
	glRectf(x + 130, y - 10, x + 150, y - 30);
	glRectf(x + 140, y, x + 180, y - 20);
	glRectf(x + 170, y - 10, x + 190, y - 30);
	glRectf(x + 180, y - 20, x + 200, y - 90);
	glRectf(x + 130, y - 80, x + 150, y - 100);
	glRectf(x + 140, y - 90, x + 180, y - 110);
	glRectf(x + 170, y - 80, x + 190, y - 100);

	// S
	glRectf(x + 230, y - 10, x + 250, y - 30);
	glRectf(x + 240, y, x + 290, y - 20);
	glRectf(x + 280, y - 10, x + 300, y - 30);
	glRectf(x + 230, y - 30, x + 250, y - 55);
	glRectf(x + 240, y - 45, x + 290, y - 65);
	glRectf(x + 280, y - 55, x + 300, y - 95);
	glRectf(x + 230, y - 80, x + 250, y - 100);
	glRectf(x + 240, y - 90, x + 290, y - 110);
	glRectf(x + 280, y - 80, x + 300, y - 100);

	// E
	glRectf(x + 330, y, x + 350, y - 110);
	glRectf(x + 330, y, x + 400, y - 20);
	glRectf(x + 330, y - 45, x + 400, y - 65);
	glRectf(x + 330, y - 90, x + 400, y - 110);
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

	for(auto& p : player)
	{
		if (p.GetConnected())
		{
			glPushMatrix();
			glTranslatef(p.GetX(), p.GetY(), 0);
			glRotatef(p.GetRotate(), 0, 0, 1);
			glTranslatef(-p.GetX(), -p.GetY(), 0);
			glPushMatrix();
			p.DrawPlayer();
			weapon.DrawWeapon();
			glPopMatrix();
			glPopMatrix();
		}
	}

	glPushMatrix();
		for (auto& b : bullets)
			b.DrawBullet();
	glPopMatrix();

	glPushMatrix();
	map.DrawMap();

	if (show_winner && !get_ready)
	{
		if(winner_id == myID) Win();
		else Lose();
	}

	if (!game_start && get_ready) Ready();

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
	sockAddr.sin_addr.s_addr = inet_addr(ip.c_str());
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

		// 마우스 처리
		glutMouseFunc(ProcessMouse);
		glutPassiveMotionFunc(ProcessMouseMotion);

		g_prevTimeInMillisecond = glutGet(GLUT_ELAPSED_TIME);

		glutTimerFunc(1, Timerfunction, 1);
		glClearColor(1.f, 1.f, 1.f, 0.f);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, WIDTH, 0, HEIGHT);

		glutMainLoop();
	}
	else return;
}

int main(int argc, char** argv)
{
	// 접속
	std::cout << "******************************" << std::endl;
	std::cout << "         Shooting Nemo        " << std::endl;
	std::cout << "******************************" << std::endl;
	std::cout << "사용할 무기의 종류를 선택해 주세요(1 - 강하지만 느린 무기, 2 - 약하지만 빠른 무기): ";
	std::cin >> myWeapon;
	std::cout << "Server IP 주소를 입력해 주세요: ";
	std::cin >> ip;


	std::wcout.imbue(std::locale("korean"));
	bullets.reserve(100);
	InitOpenGL(argc, argv);
	closesocket(cSocket);
	WSACleanup();
	return 0;
}