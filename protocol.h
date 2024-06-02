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
constexpr char CS_MOVE = 1;
constexpr char CS_SKILL = 2;


// Server -> Client
constexpr char SC_LOGIN_INFO = 11;
constexpr char SC_ADD_PLAYER = 12;
constexpr char SC_REMOVE_PLAYER = 13;
constexpr char SC_MOVE_PLAYER = 14;
constexpr char SC_SKILL_PLAYER = 15;

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

// 현재 무기 및 스킬 키 인풋, 사용 스킬
struct CS_SKILL_PACKET {
	unsigned char size;
	char	type;    
	uint8_t	Weapon_N_Attack;		// 4비트 WeaponType | 4비트 AttackType
	uint8_t Animation;				// current Animation state
};

// 이동 및 스킬 사용 키인풋 패킷
struct CS_MOVE_PACKET {
	unsigned char size;
	char	type;    
	uint8_t	keyinput;  
	short	camera_yaw, player_yaw;	// rotation을 위해 필요
};



///////////////////////////////////////////////
// Server -> Client
///////////////////////////////////////////////

// 클라이언트 개인 서버에 접속한 플레이어 정보
struct SC_LOGIN_INFO_PACKET {
	unsigned char size;
	char	type;
	int 	id;
	float 	x, y, z;
};

// 클라이언트 개인 서버에 접속한 플레이어 정보
struct SC_ADD_PLAYER_PACKET {
	unsigned char size;
	char	type;
	int 	id;
	float 	x, y, z;
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
	float 	x, y, z;		// 움직인 위치
	float	yaw;	// look vector
};

// 현재 무기 및 스킬 키 인풋, 사용 스킬
struct SC_SKILL_PACKET {
	unsigned char size;
	char	type;
	int		id;
	uint8_t	Weapon_N_Attack;		// 4비트 WeaponType | 4비트 AttackType
	uint8_t Animation;				// current Animation state
};
#pragma pack (pop)