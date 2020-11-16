#pragma once
#include <chrono>
///////////////////////////////////////////////////////////////////////////////
#define ctos_login			20
#define ctos_shoot			21
#define ctos_move			22
#define ctos_ready			24
#define ctos_weapon_select	25
#define stoc_new_client		26
#define stoc_bullets		27
#define stoc_move			28
#define stoc_hit			29
#define stoc_ready			30
#define stoc_gamestart		31
#define stoc_weapon_select	32
#define stoc_world_state	33
///////////////////////////////////////////////////////////////////////////////
#define DIR_UP			10
#define DIR_DOWN 		11
#define DIR_LEFT 		12
#define DIR_RIGHT 		13
#define STRONG_WEAPON	14
#define NORMAL_WAEPON	15
///////////////////////////////////////////////////////////////////////////////
#define SERVER_PORT 5959
#define MAX_BUFFER 5120
///////////////////////////////////////////////////////////////////////////////
#define PLAYER_X_SPEED 0.5
#define PLAYER_Y_SPEED 0.5
#define PLAYER_SIZE 0.1
///////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)

//STOC_WORLD_STATE 구조체에 담을 클라이언트들의 현재 정보를 담은 구조체
struct CLIENT_STATE {
	float x;
	float y;
	int hp;
	bool is_connected;
};

//CTOS_*은 CLIENT TO SERVER의 줄임말로, CLIENT에서 SERVER로 보내는 패킷을 의미한다.
//STOC_*은 SERVER TO CLIENT의 줄임말로, SERVER에서 CLIENT로 보내는 패킷을 의미한다.
struct CTOS_LOGIN {
	unsigned char size;
	unsigned char type;
};

struct CTOS_SHOOT {
	unsigned char size;
	unsigned char type;
	unsigned char id;
	unsigned char weapon_type;
	unsigned char dir;
};

struct CTOS_MOVE{
	unsigned char size;
	unsigned char type;
	unsigned char id;
	unsigned char dir;
	std::chrono::system_clock::time_point time;
};

struct CTOS_READY {
	unsigned char size;
	unsigned char type;
	unsigned char id;
};

struct CTOS_WEAPON_SELECT {
	unsigned char size;
	unsigned char type;
	unsigned char id;
	unsigned char weapon_type;
};


struct STOC_NEW_CLIENT {
	unsigned char size;
	unsigned char type;
	unsigned char id;
	float x;
	float y;
	float player_size;
};

struct STOC_MOVE {
	unsigned char size;
	unsigned char type;
	unsigned char id;
	short x;
	short y;
};

struct STOC_HIT {
	unsigned char size;
	unsigned char type;
	unsigned char id;
	unsigned char hp;
};

struct STOC_BULLETS_INFO {
	unsigned char size;
	unsigned char type;
};

struct STOC_READY {
	unsigned char size;
	unsigned char type;
	unsigned char id;
};

struct STOC_GAMESTART {
	unsigned char size;
	unsigned char type;
};

struct STOC_WEAPON_SELECT {
	unsigned char size;
	unsigned char type;
	unsigned char id;
	unsigned char weapon_type;
};

struct STOC_WORLD_STATE {
	unsigned char size;
	unsigned char type;
	CLIENT_STATE clients_state[2];
};
#pragma pack(pop)
///////////////////////////////////////////////////////////////////////////////