#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "Player.h"
#include "Bullet.h"
#include "../../protocol.h"
#pragma comment (lib, "ws2_32")
#define MIN_BUFFER_SIZE 1024
using namespace std;
using namespace chrono;

int user_id = 0;
int conn_clients = 0;
Player clients[2];
CLIENT_STATE clients_state[2];
vector<Bullet> bullets;
mutex b_mx;
float starting_point[4]{ 300.f, 300.f, 900.f, 300.f };
chrono::system_clock::time_point prev_time;

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

void CollisionPlayerwithMap(int id)
{
	float player_x = clients[id].GetX();
	float player_y = clients[id].GetY();
	float radius = clients[id].GetRadius();

	if (player_x - radius < 0)
	{
		float over_x = 0.f - (player_x - radius);
		clients[id].SetX(player_x + over_x);
		return;
	}
	if (player_x + radius > 1200)
	{
		float over_x = (player_x + radius) - 1200.f;
		clients[id].SetX(player_x - over_x);
		return;
	}
	if (player_y - radius < 0)
	{
		float over_y = 0.f - (player_y - radius);
		clients[id].SetY(player_y + over_y);
		return;
	}
	if (player_y + radius > 800)
	{
		float over_y = (player_y + radius) - 800.f;
		clients[id].SetY(player_y - over_y);
		return;
	}
}

void CollisionBulletwithPlayer(int id)
{
	int other_id = 1 - id;

	float bullet_x = bullets[id].GetBulletX();
	float bullet_y = bullets[id].GetBulletY();
	float radian = bullets[id].GetRadian();

	float player_x = clients[other_id].GetX();
	float player_y = clients[other_id].GetY();
	float radius = clients[other_id].GetRadius();

	float distance = sqrt(pow((bullet_x) - (player_x + radius), 2) + pow((bullet_y) - (player_y + radius), 2));

	//if (distance < player_x)
	//{
	//	std::cout << "충돌 " << std::endl;
	//}
	//else
	//{
	//	return;
	//	std::cout << "no" << std::endl;
	//}
	//if (bullet_x - radian < 0)
	//{
	//	return;
	//}
	//if (bullet_x + radian > 1200)
	//{
	//	return;
	//}
	//if (bullet_y - radian < 0)
	//{
	//	return;
	//}
	//if (bullet_y - radian > 800)
	//{
	//	return;
	//}
}

void process_packet(int id)
{
	char* packet = clients[id].GetPacketStartPtr();
	char type = packet[2];
	switch (type)
	{
	case ctos_move:
		{
			CTOS_MOVE* p = reinterpret_cast<CTOS_MOVE*>(packet);
			auto dir = p->dir;
			
			if (dir == DIR_UP || dir == DIR_DOWN_UP)
			{	
				clients[id].SetYSpeed(PLAYER_Y_SPEED);
			}
			else if (dir == DIR_DOWN || dir == DIR_UP_UP)
			{
				clients[id].SetYSpeed(-PLAYER_Y_SPEED);
			}
			else if (dir == DIR_LEFT || dir == DIR_RIGHT_UP)
			{
				clients[id].SetXSpeed(-PLAYER_X_SPEED);
			}
			else if (dir == DIR_RIGHT || dir == DIR_LEFT_UP)
			{
				clients[id].SetXSpeed(PLAYER_X_SPEED);
			}
			break;
		}
	case ctos_shoot:
		{
			CTOS_SHOOT* p = reinterpret_cast<CTOS_SHOOT*> (packet);
			b_mx.lock();
			bullets.emplace_back(p->x, p->y, p->angle, p->radian, p->id);
			b_mx.unlock();
			break;
		}
	case ctos_rotate:
		{
			CTOS_ROTATE* p = reinterpret_cast<CTOS_ROTATE*> (packet);
			clients[p->id].SetRotate(p->rotate);
			break;
		}
	}
}

void UpdateAndSendThread()
{
	while (true)
	{
		STOC_WORLD_STATE p;
		p.type = stoc_world_state;

		auto start_time = system_clock::now();
		duration<double> frame_time = start_time - prev_time;

		//플레이어 정보 저장
		for (int i=0;i<2;++i)
		{
			clients[i].Lock();
			if (clients[i].IsConnected())
			{
				clients[i].Update(frame_time);
				CollisionPlayerwithMap(i);
				p.clients_state[i].hp = clients[i].GetHP();
				p.clients_state[i].x = clients[i].GetX();
				p.clients_state[i].y = clients[i].GetY();
				p.clients_state[i].is_connected = true;
				p.clients_state[i].rotate = clients[i].GetRotate();
			}
			else
			{
				p.clients_state[i].hp = NULL;
				p.clients_state[i].x = NULL;
				p.clients_state[i].y = NULL;
				p.clients_state[i].is_connected = false;
				p.clients_state[i].rotate = NULL;
			}
			clients[i].Unlock();
		}

		//총알정보 업데이트 및 저장
		int i = 0;
		for (auto& b : bullets)
		{
			b.Update(frame_time);
			CollisionBulletwithPlayer(i);
			p.bullets_state[i].x = b.GetStartX();
			p.bullets_state[i].y = b.GetStartY();
			p.bullets_state[i].angle = b.GetAngle();
			p.bullets_state[i].speed = b.GetSpeed();
			i += 1;
		}
		bullets.erase(remove_if(bullets.begin(),
								bullets.end(),
								[](Bullet& b) {return !b.GetActive(); }),
					  bullets.end());
		cout << bullets.size() << endl;
		p.b_num = bullets.size();
		p.size = sizeof(p) - (sizeof(BULLET_STATE) * (200 - p.b_num));

		//전송
		for (auto& cl : clients)
			cl.SendPacket(reinterpret_cast<void*>(&p), p.size);

		auto end_time = system_clock::now();
		prev_time = end_time;

		auto elapsed_time = duration_cast<milliseconds>(end_time - start_time);

		this_thread::sleep_for(16ms - elapsed_time);
	}
}

void RecvThread(int id)
{
	while (true)
	{
		int recv_bytes = clients[id].Recv();
		if (recv_bytes == 0)
		{
			//closesocket
		}
		else if (recv_bytes == SOCKET_ERROR)
		{
			if (WSAGetLastError() == 10054)
			{
				clients[id].~Player();
				conn_clients -= 1;
				return;
			}
			string error_str = "Player" + to_string(id) + "recv error";
			error_display(error_str.c_str(), WSAGetLastError());
		}
		else
		{
			char* buf = clients[id].GetPacketStartPtr();
			short p_size = *reinterpret_cast<short*>(&buf[0]);
			char* next_recv_point = clients[id].GetRecvStartPtr() + recv_bytes;
			while (p_size <= next_recv_point - buf)
			{
				process_packet(id);
				buf += p_size;
				clients[id].SetPacketStartPtr(buf);

				//여러개의 패킷이 한번에 수신된 상황일 경우 다음 패킷의 크기를 구한다.
				if (buf < next_recv_point)
					p_size = *reinterpret_cast<short*>(&buf[0]);
				else break;
			}

			int left_data = next_recv_point - buf;

			/***************************************************
			버퍼에 남아있는 공간이 MIN_BUFFER_SIZE보다 작으면
			아직 처리하지 않은 데이터들을 버퍼의 맨 앞으로 이동시킨다.
			****************************************************/
			if (MAX_BUFFER - (next_recv_point - clients[id].GetBuffer()) < MIN_BUFFER_SIZE)
			{
				memcpy(clients[id].GetBuffer(), buf, left_data);
				clients[id].ResetPacketStartPtr();
				next_recv_point = clients[id].GetPacketStartPtr() + left_data;
			}
			clients[id].SetRecvStart(next_recv_point);
		}
	}
}

int main()
{
	//display_error호출 시 정상적으로 한글 출력을 하기 위함.
	std::wcout.imbue(std::locale("korean"));
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	bullets.reserve(10000);

	SOCKET lSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(SERVER_PORT);
	sockAddr.sin_addr.s_addr = ADDR_ANY;
	::bind(lSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	listen(lSocket, SOMAXCONN);

	prev_time = chrono::system_clock::now();
	auto update_thread = thread{ UpdateAndSendThread };
	thread recv_thread[2];
	sockaddr_in stoc_sockAddr;
	int addrSize = sizeof(stoc_sockAddr);

	//accept처리
	while (true)
	{
		if (conn_clients < 2)
		{
			auto stoc_socket = accept(lSocket, (SOCKADDR*)&stoc_sockAddr, &addrSize);
			clients[user_id].PlayerInit(stoc_socket, user_id);
			recv_thread[user_id] = thread{ RecvThread, user_id };

			//접속한 클라이언트의 정보를 패킷에 저장한다.
			STOC_LOGIN_OK p;
			p.type = stoc_login_ok;
			p.size = sizeof(p);
			p.id = clients[user_id].GetID();
			p.x = clients[user_id].GetX();
			p.y = clients[user_id].GetY();
			p.player_size = clients[user_id].GetSize();

			//접속한 클라이언트 정보를 모든 클라이언트에게 전송한다.
			for (auto& cl : clients)
			{
				int id = cl.GetID();
				if (id == user_id)
				{
					cl.SendPacket(reinterpret_cast<void*>(&p), sizeof(p));
					//이미 접속중인 클라이언트가 있으면 해당 클라이언트 정보를 전송한다.
					int other_id = 1 - id;
					if (clients[other_id].IsConnected())
					{
						STOC_NEW_CLIENT other_cl;
						other_cl.id = other_id;
						other_cl.x = clients[other_id].GetX();
						other_cl.y = clients[other_id].GetY();
						other_cl.size = sizeof(other_cl);
						other_cl.player_size = clients[other_id].GetSize();
						other_cl.type = stoc_new_client;
						cl.SendPacket(reinterpret_cast<void*>(&other_cl), sizeof(other_cl));
					}
				}
				else
				{
					STOC_NEW_CLIENT new_user;
					new_user.id = user_id;
					new_user.x = clients[user_id].GetX();
					new_user.y = clients[user_id].GetY();
					new_user.size = sizeof(new_user);
					new_user.player_size = clients[user_id].GetSize();
					new_user.type = stoc_new_client;
					cl.SendPacket(reinterpret_cast<void*>(&new_user), sizeof(new_user));
				}
			}
			user_id = (user_id + 1) % 2;
			++conn_clients;
		}
	}

	//다른 스레드가 종료될 때 까지 기다린다.
	for (auto& th : recv_thread)
		th.join();
	update_thread.join();

	WSACleanup();
}