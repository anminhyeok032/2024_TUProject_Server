#pragma once

constexpr int PORT_NUM = 4000;
constexpr int BUF_SIZE = 200;
constexpr int NAME_SIZE = 25;
constexpr int PASSWORD_SIZE = 25;

constexpr int W_WIDTH = 8;
constexpr int W_HEIGHT = 8;

#define MAXUSER = 4;

// Packet ID
// Client -> Server
constexpr char CS_LOGIN = 0;
constexpr char CS_MOUSE = 1;
constexpr char CS_KEYINPUT = 2;

// Server -> Client
constexpr char SC_LOGIN_INFO = 11;
constexpr char SC_ADD_PLAYER = 12;
constexpr char SC_REMOVE_PLAYER = 13;
constexpr char SC_MOVE_PLAYER = 14;

#pragma pack (push, 1)

///////////////////////////////////////////////
// Client -> Server
///////////////////////////////////////////////

// 로그인시 패킷
struct CS_LOGIN_PACKET {
	unsigned char size;
	char	type;
	int 	id;
	char	name[NAME_SIZE];
	char	password[PASSWORD_SIZE];
};

// TODO: 해당 패킷은 마우스 방향을 돌릴때 캐릭터가 어떻게 움직이는지에 따라 바뀔 예정
// 이동 및 스킬 사용 키인풋 패킷
struct CS_MOUSE_PACKET {
	unsigned char size;
	char	type;       
	float	Rotation;		// 마우스 방향에 따른 캐릭터가 바라보는 방향 
};

// 이동 및 스킬 사용 키인풋 패킷
struct CS_KEYINPUT_PACKET {
	unsigned char size;
	char	type;       
	char	direction;  // 0 : UP, 1 : DOWN, 2 : LEFT, 3 : RIGHT, 4~ SKILL
};

///////////////////////////////////////////////
// Server -> Client
///////////////////////////////////////////////

// 클라이언트 개인 서버에 접속한 플레이어 정보
struct SC_LOGIN_INFO_PACKET {
	unsigned char size;
	char	type;
	int 	id;
	int 	x, y, z;
};

// 클라이언트 개인 서버에 접속한 플레이어 정보
struct SC_ADD_PLAYER_PACKET {
	unsigned char size;
	char	type;
	int 	id;
	int 	x, y, z;
	char	nickname[NAME_SIZE];
};

// 클라이언트 개인 서버에 접속한 플레이어 정보
struct SC_REMOVE_PLAYER_PACKET {
	unsigned char size;
	char	type;		
	int		id;
};

struct SC_MOVE_PLAYER_PACKET {
	unsigned char size;
	char	type;		
	int		id;				// 서버에 접속해 있는 캐릭터 번호
	int 	x, y, z;		// 움직인 위치
};

#pragma pack (pop)