#pragma once
#include <WS2tcpip.h>
#include <mutex>
#include "../../protocol.h"
using namespace std;

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
	float radius;
	float x_speed;
	float y_speed;
	int hp;
	bool is_connected;
	mutex mx;

	float m_Rotate;

public:
	Player() { is_connected = false; x_speed = 0; y_speed = 0; }
	~Player() { closesocket(stoc_socket); is_connected = false; }
	void PlayerInit(SOCKET& sock, int playerID);
	void Reset();

	int GetID() { return id; }
	int GetHP() { return hp; }
	float GetX() { return x; }
	float GetY() { return y; }
	float GetXSpeed() { return x_speed; }
	float GetYSpeed() { return y_speed; }
	float GetSize() { return size; }
	float GetRotate() { return m_Rotate; }
	float GetRadius() { return radius; }
	char* GetBuffer() { return recvBuf; }
	char* GetPacketStartPtr() { return packet_start_ptr; }
	char* GetRecvStartPtr() { return recv_start_ptr; }
	bool IsConnected() { return is_connected; }

	void SetID(int playerID) { id = playerID; }
	void SetHP(int playerHP) { hp = playerHP; }
	void SetX(float xPos) { x = xPos; }
	void SetY(float yPos) { y = yPos; }
	void SetXSpeed(float speed) { x_speed += speed; }
	void SetYSpeed(float speed) { y_speed += speed; }
	void SetSize(float playerSize) { size = playerSize; }
	void SetRotate(float rotate) { m_Rotate = rotate; }
	void SetRecvStart(char* new_start_ptr) { recv_start_ptr = new_start_ptr; };
	void SetPacketStartPtr(char* new_packet_start_ptr) { packet_start_ptr = new_packet_start_ptr; }
	void ResetPacketStartPtr() { packet_start_ptr = recv_start_ptr; }

	void Update(std::chrono::duration<double>& time);

	void Lock() { mx.lock(); }
	void Unlock() { mx.unlock(); }

	int Recv();
	int SendPacket(void* packet, int p_size);
};

