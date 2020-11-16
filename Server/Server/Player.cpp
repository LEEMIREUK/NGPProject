#include "Player.h"
extern float starting_point[4];

void Player::PlayerInit(SOCKET& sock, int playerID)
{
	stoc_socket = sock;
	id = playerID;
	size = PLAYER_SIZE;
	hp = 100;
	if (playerID == 0)
	{
		x = starting_point[0];
		y = starting_point[1];
	}
	else
	{
		x = starting_point[2];
		y = starting_point[3];
	}
	is_connected = true;
	recv_start_ptr = recvBuf;
	packet_start_ptr = recvBuf;
}

int Player::Recv()
{
	return recv(stoc_socket, recv_start_ptr, MAX_BUFFER, 0);
}

int Player::SendPacket(void* packet, int p_size)
{
	char* p = reinterpret_cast<char*>(packet);
	return send(stoc_socket, p, p_size, 0);
}
