#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include "Player.h"
#include "../../protocol.h"
#pragma comment (lib, "ws2_32")
#define MIN_BUFFER_SIZE 1024
using namespace std;
using namespace chrono;

int user_id = 0;
Player clients[2];
CLIENT_STATE clients_state[2];
float starting_point[4]{ -0.7f, 0.f, 0.7f, 0.f };

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
	std::wcout << L"���� (" << err_no << ") " << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

void process_shoot(int id)
{

}

void process_packet(int id)
{
	char* packet = clients[id].GetPacketStartPtr();
	char type = packet[1];
	switch (type)
	{
	case ctos_move:
		{
			CTOS_MOVE* p = reinterpret_cast<CTOS_MOVE*>(packet);
			auto dir = p->dir;
			auto cur_time = system_clock::now();
			auto elapsed_time = (cur_time - p->time).count();
			
			if (dir == DIR_UP)
			{
				auto curY = clients[id].GetY();
				curY += PLAYER_Y_SPEED * elapsed_time;
				clients[id].SetY(curY);
			}
			else if (dir == DIR_DOWN)
			{
				auto curY = clients[id].GetY();
				curY -= PLAYER_Y_SPEED * elapsed_time;
				clients[id].SetY(curY);
			}
			else if (dir == DIR_LEFT)
			{
				auto curX = clients[id].GetX();
				curX -= PLAYER_X_SPEED * elapsed_time;
				clients[id].SetX(curX);
			}
			else if (dir == DIR_RIGHT)
			{
				auto curX = clients[id].GetX();
				curX += PLAYER_X_SPEED * elapsed_time;
				clients[id].SetX(curX);
			}
			break;
		}
	case ctos_shoot:
		{
			process_shoot(id);
			break;
		}
	}
}

void UpdateAndSendThread()
{
	while (true)
	{
		STOC_WORLD_STATE p;
		p.size = sizeof(p);
		p.type = stoc_world_state;
		for (int i=0;i<2;++i)
		{
			if (clients[i].IsConnected())
			{
				p.clients_state[i].hp = clients[i].GetHP();
				p.clients_state[i].x = clients[i].GetX();
				p.clients_state[i].y = clients[i].GetY();
				p.clients_state->is_connected = true;
			}
			else
			{
				p.clients_state[i].hp = NULL;
				p.clients_state[i].x = NULL;
				p.clients_state[i].y = NULL;
				p.clients_state->is_connected = false;
			}
		}

		for (auto& cl : clients)
			cl.SendPacket(reinterpret_cast<void*>(&p), p.size);
	}
}

void RecvThread(int id)
{
	while (true)
	{
		int recv_bytes = clients[id].Recv();
		if (recv_bytes == 0)
			return;
		else if (recv_bytes == SOCKET_ERROR)
		{
			string error_str = "Player" + to_string(id) + "recv error";
			error_display(error_str.c_str(), WSAGetLastError());
		}
		else
		{
			char* buf = clients[id].GetPacketStartPtr();
			unsigned char p_size = buf[0];
			char* next_recv_point = clients[id].GetRecvStartPtr() + recv_bytes;
			while (p_size <= next_recv_point - buf)
			{
				process_packet(id);
				buf += p_size;
				clients[id].SetPacketStartPtr(buf);

				//�������� ��Ŷ�� �ѹ��� ���ŵ� ��Ȳ�� ��� ���� ��Ŷ�� ũ�⸦ ���Ѵ�.
				if (buf < next_recv_point)
					p_size = buf[0];
				else break;
			}

			int left_data = next_recv_point - buf;

			/***************************************************
			���ۿ� �����ִ� ������ MIN_BUFFER_SIZE���� ������
			���� ó������ ���� �����͵��� ������ �� ������ �̵���Ų��.
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
	//display_errorȣ�� �� ���������� �ѱ� ����� �ϱ� ����.
	std::wcout.imbue(std::locale("korean"));

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	SOCKET lSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(SERVER_PORT);
	sockAddr.sin_addr.s_addr = ADDR_ANY;
	::bind(lSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	listen(lSocket, SOMAXCONN);

	auto update_thread = thread{ UpdateAndSendThread };
	thread recv_thread[2];
	sockaddr_in stoc_sockAddr;
	int addrSize = sizeof(stoc_sockAddr);

	//acceptó��
	while (true)
	{
		auto stoc_socket = accept(lSocket, (SOCKADDR*)&stoc_sockAddr, &addrSize);
		clients[user_id].PlayerInit(stoc_socket, user_id);
		recv_thread[user_id] = thread{ RecvThread, user_id };

		//������ Ŭ���̾�Ʈ�� ������ ��Ŷ�� �����Ѵ�.
		STOC_NEW_CLIENT p;
		p.type = stoc_new_client;
		p.size = sizeof(p);
		p.id = clients[user_id].GetID();
		p.x = clients[user_id].GetX();
		p.y = clients[user_id].GetY();
		p.player_size = clients[user_id].GetSize();
		
		//������ Ŭ���̾�Ʈ ������ ��� Ŭ���̾�Ʈ���� �����Ѵ�.
		for (auto& cl : clients)
			cl.SendPacket(reinterpret_cast<void*>(&p), p.size);
		++user_id;
	}

	//�ٸ� �����尡 ����� �� ���� ��ٸ���.
	for (auto& th : recv_thread)
		th.join();
	update_thread.join();
}