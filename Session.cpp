#include "Session.h"

void SESSION::do_recv()
{
	DWORD recv_flag = 0;
	memset(&_recv_over._over, 0, sizeof(_recv_over._over));
	// ��Ŷ �߸� ���� ���
	_recv_over._wsabuf.len = BUF_SIZE - _prev_remain;
	_recv_over._wsabuf.buf = _recv_over._send_buf + _prev_remain;
	WSARecv(_socket, &_recv_over._wsabuf, 1, 0, &recv_flag,
		&_recv_over._over, 0);
}


void SESSION::do_send(void* packet)
{
	OVER_EXP* sdata = new OVER_EXP{ reinterpret_cast<unsigned char*>(packet) };
	WSASend(_socket, &sdata->_wsabuf, 1, 0, 0, &sdata->_over, 0);
}

void SESSION::send_login_info_packet()
{
	// ��Ŷ ������� �� �����ؼ� do_send���� ������
	SC_LOGIN_INFO_PACKET p;
	p.id = _id;
	p.size = sizeof(p);
	p.type = SC_LOGIN_INFO;
	p.x = x;
	p.y = y;
	do_send(&p);
}

void SESSION::send_move_packet(int c_id)
{
	// TODO : ������ ��Ŷ ���� z �൵ ����ؾ���
	SC_MOVE_PLAYER_PACKET p;
	p.id = c_id;
	p.size = sizeof(SC_MOVE_PLAYER_PACKET);
	p.type = SC_MOVE_PLAYER;
	p.x = clients[c_id].x;
	p.y = clients[c_id].y;
	do_send(&p);
}
