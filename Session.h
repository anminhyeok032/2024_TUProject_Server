#pragma once
#include "global.h"
#include "protocol.h"
#include "Exp_Over.h"

class SESSION {
	OVER_EXP _recv_over;

public:
	bool in_use;
	int _id;
	SOCKET _socket;
	short	x, y;
	char	_name[NAME_SIZE];

	int		_prev_remain;
public:
	SESSION() : _socket(0), in_use(false)
	{
		_id = -1;
		x = y = 0;
		_name[0] = 0;
		_prev_remain = 0;
	}

	~SESSION() {}

	void do_recv()
	{
		DWORD recv_flag = 0;
		memset(&_recv_over._over, 0, sizeof(_recv_over._over));
		// 패킷 잘린 조각 고려
		_recv_over._wsabuf.len = BUF_SIZE - _prev_remain;
		_recv_over._wsabuf.buf = _recv_over._send_buf + _prev_remain;
		WSARecv(_socket, &_recv_over._wsabuf, 1, 0, &recv_flag,
			&_recv_over._over, 0);
	}

	void do_send(void* packet)
	{
		OVER_EXP* sdata = new OVER_EXP{ reinterpret_cast<unsigned char*>(packet) };
		WSASend(_socket, &sdata->_wsabuf, 1, 0, 0, &sdata->_over, 0);
	}

	void send_login_info_packet()
	{
		// 패킷 사라지는 걸 방지해서 do_send에서 복사함
		SC_LOGIN_INFO_PACKET p;
		p.id = _id;
		p.size = sizeof(p);
		p.type = SC_LOGIN_INFO;
		p.x = x;
		p.y = y;
		do_send(&p);
	}

	void send_move_packet(int c_id);
};
