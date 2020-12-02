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
#define stoc_login_ok		34
///////////////////////////////////////////////////////////////////////////////
#define DIR_UP			10
#define DIR_DOWN 		11
#define DIR_LEFT 		12
#define DIR_RIGHT 		13
#define DIR_UP_UP		14
#define DIR_DOWN_UP		15
#define DIR_LEFT_UP 	16
#define DIR_RIGHT_UP 	17
#define STRONG_WEAPON	18
#define NORMAL_WAEPON	19
///////////////////////////////////////////////////////////////////////////////
#define SERVER_PORT 5959
#define MAX_BUFFER 5120
///////////////////////////////////////////////////////////////////////////////
#define PLAYER_X_SPEED 300.f
#define PLAYER_Y_SPEED 300.f
#define BULLET_SPEED 500.f
#define PLAYER_SIZE 25.f
///////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)

//STOC_WORLD_STATE ����ü�� ���� Ŭ���̾�Ʈ���� ���� ������ ���� ����ü
struct CLIENT_STATE {
	float x;
	float y;
	int hp;
	bool is_connected;
};

struct BULLET_STATE {
	float x;
	float y;
	float angle;
	float speed;
};

//CTOS_*�� CLIENT TO SERVER�� ���Ӹ���, CLIENT���� SERVER�� ������ ��Ŷ�� �ǹ��Ѵ�.
//STOC_*�� SERVER TO CLIENT�� ���Ӹ���, SERVER���� CLIENT�� ������ ��Ŷ�� �ǹ��Ѵ�.
struct CTOS_LOGIN {
	short size;
	unsigned char type;
};

struct CTOS_SHOOT {
	short size;
	unsigned char type;
	unsigned char id;
	float x;
	float y;
	float angle;
	float radian;
};

struct CTOS_MOVE{
	short size;
	unsigned char type;
	unsigned char id;
	unsigned char dir;
};

struct CTOS_READY {
	short size;
	unsigned char type;
	unsigned char id;
};

struct CTOS_WEAPON_SELECT {
	short size;
	unsigned char type;
	unsigned char id;
	unsigned char weapon_type;
};

struct STOC_LOGIN_OK {
	short size;
	unsigned char type;
	unsigned char id;
	float x;
	float y;
	float player_size;
};

struct STOC_NEW_CLIENT {
	short size;
	unsigned char type;
	unsigned char id;
	float x;
	float y;
	float player_size;
};

struct STOC_MOVE {
	short size;
	unsigned char type;
	unsigned char id;
	float x;
	float y;
};

struct STOC_HIT {
	short size;
	unsigned char type;
	unsigned char id;
	unsigned char hp;
};

struct STOC_BULLETS_INFO {
	short size;
	unsigned char type;
};

struct STOC_READY {
	short size;
	unsigned char type;
	unsigned char id;
};

struct STOC_GAMESTART {
	short size;
	unsigned char type;
};

struct STOC_WEAPON_SELECT {
	short size;
	unsigned char type;
	unsigned char id;
	unsigned char weapon_type;
};

struct STOC_WORLD_STATE {
	short size;
	unsigned char type;
	unsigned char b_num;
	CLIENT_STATE clients_state[2];
	BULLET_STATE bullets_state[200];
};
#pragma pack(pop)
///////////////////////////////////////////////////////////////////////////////