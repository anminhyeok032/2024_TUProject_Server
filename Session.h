#pragma once
#include "global.h"
#include "protocol.h"
#include "Exp_Over.h"

class SESSION {
	OVER_EXP _recv_over;

public:
	std::atomic<S_STATE> in_use;
	int _id;
	SOCKET _socket;
	// *주의 움직임 범위가 32'767이상 넘어갈 시 단위 변환 할 것
	short	x, y, z;
	char	_name[NAME_SIZE];

	// 잘린 조각을 고려하기 위한 previous remaining 변수
	int		_prev_remain;
public:
	SESSION() : _socket(0), in_use(S_STATE::ST_FREE)
	{
		_id = -1;
		x = y = 0;
		_name[0] = 0;
		_prev_remain = 0;
	}

	~SESSION() {}

	void do_recv();

	void do_send(void* packet);

	void send_login_info_packet();

	void send_add_player_packet(int c_id);

	void send_move_packet(int c_id);
};


extern std::array<SESSION, MAX_USER> clients;