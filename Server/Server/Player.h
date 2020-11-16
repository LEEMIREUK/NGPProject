#pragma once
#include <WS2tcpip.h>
#include "../../protocol.h"

class Player
{
	SOCKET stoc_socket;
	int id;
	char recvBuf[MAX_BUFFER];
	char* recv_start_ptr;
	char* packet_start_ptr;

	float x;
	float y;
	float size;
	int hp;
	bool is_connected;
public:
	Player() { is_connected = false; };
	~Player() { closesocket(stoc_socket); }
	void PlayerInit(SOCKET& sock, int playerID);

	int GetID() { return id; }
	int GetHP() { return hp; }
	float GetX() { return x; }
	float GetY() { return y; }
	float GetSize() { return size; }
	char* GetBuffer() { return recvBuf; }
	char* GetPacketStartPtr() { return packet_start_ptr; }
	char* GetRecvStartPtr() { return recv_start_ptr; }
	bool IsConnected() { return is_connected; }

	void SetID(int playerID) { id = playerID; }
	void SetHP(int playerHP) { hp = playerHP; }
	void SetX(float xPos) { x = xPos; }
	void SetY(float yPos) { y = yPos; }
	void SetSize(float playerSize) { size = playerSize; }
	void SetRecvStart(char* new_start_ptr) { recv_start_ptr = new_start_ptr; };
	void SetPacketStartPtr(char* new_packet_start_ptr) { packet_start_ptr = new_packet_start_ptr; }
	void ResetPacketStartPtr() { packet_start_ptr = recv_start_ptr; }

	int Recv();
	int SendPacket(void* packet, int p_size);
};

