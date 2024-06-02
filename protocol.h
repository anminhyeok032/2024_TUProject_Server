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

// �α��ν� ��Ŷ
struct CS_LOGIN_PACKET {
	unsigned char size;
	char	type;
	int 	id;
	char	name[NAME_SIZE];
	char	password[PASSWORD_SIZE];
};

// ���� ���� �� ��ų Ű ��ǲ, ��� ��ų
struct CS_SKILL_PACKET {
	unsigned char size;
	char	type;    
	uint8_t	Weapon_N_Attack;		// 4��Ʈ WeaponType | 4��Ʈ AttackType
	uint8_t Animation;				// current Animation state
};

// �̵� �� ��ų ��� Ű��ǲ ��Ŷ
struct CS_MOVE_PACKET {
	unsigned char size;
	char	type;    
	uint8_t	keyinput;  
	short	camera_yaw, player_yaw;	// rotation�� ���� �ʿ�
};



///////////////////////////////////////////////
// Server -> Client
///////////////////////////////////////////////

// Ŭ���̾�Ʈ ���� ������ ������ �÷��̾� ����
struct SC_LOGIN_INFO_PACKET {
	unsigned char size;
	char	type;
	int 	id;
	float 	x, y, z;
};

// Ŭ���̾�Ʈ ���� ������ ������ �÷��̾� ����
struct SC_ADD_PLAYER_PACKET {
	unsigned char size;
	char	type;
	int 	id;
	float 	x, y, z;
	char	nickname[NAME_SIZE];
};

// Ŭ���̾�Ʈ ���� ������ ������ �÷��̾� ����
struct SC_REMOVE_PLAYER_PACKET {
	unsigned char size;
	char	type;		
	int		id;
};

struct SC_MOVE_PLAYER_PACKET {
	unsigned char size;
	char	type;		
	int		id;				// ������ ������ �ִ� ĳ���� ��ȣ
	float 	x, y, z;		// ������ ��ġ
	float	yaw;	// look vector
};

// ���� ���� �� ��ų Ű ��ǲ, ��� ��ų
struct SC_SKILL_PACKET {
	unsigned char size;
	char	type;
	int		id;
	uint8_t	Weapon_N_Attack;		// 4��Ʈ WeaponType | 4��Ʈ AttackType
	uint8_t Animation;				// current Animation state
};
#pragma pack (pop)