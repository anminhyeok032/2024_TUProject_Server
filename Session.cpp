#include "Session.h"

void print_error(const char* msg, int err_no)
{
	WCHAR* msg_buf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&msg_buf), 0, NULL);
	std::cout << msg;
	std::wcout << L" : 에러 : " << msg_buf;
	while (true);
	LocalFree(msg_buf);
}

void SESSION::do_recv()
{
	DWORD recv_flag = 0;
	memset(&_recv_over._over, 0, sizeof(_recv_over._over));
	// 패킷 잘린 조각 고려
	_recv_over._wsabuf.len = BUF_SIZE - _prev_remain;
	_recv_over._wsabuf.buf = _recv_over._send_buf + _prev_remain;
	WSARecv(_socket, &_recv_over._wsabuf, 1, 0, &recv_flag,
		&_recv_over._over, 0);
}


void SESSION::do_send(void* packet)
{
	OVER_EXP* sdata = new OVER_EXP{ reinterpret_cast<unsigned char*>(packet) };
	int res = WSASend(_socket, &sdata->_wsabuf, 1, 0, 0, &sdata->_over, 0);
	if (res != 0)
	{
		print_error("do_send", WSAGetLastError());
	}
}

void SESSION::send_login_info_packet()
{
	// 패킷 사라지는 걸 방지해서 do_send에서 복사함
	SC_LOGIN_INFO_PACKET p;
	p.id = _id;
	p.size = sizeof(p);
	p.type = SC_LOGIN_INFO;
	p.x = x;
	p.y = y;
	p.z = z;
	do_send(&p);
}

void SESSION::send_add_player_packet(int c_id)
{
	SC_ADD_PLAYER_PACKET add_packet;
	add_packet.id = c_id;
	strcpy_s(add_packet.nickname, clients[c_id]._name);
	add_packet.size = sizeof(add_packet);
	add_packet.type = SC_ADD_PLAYER;
	add_packet.x = clients[c_id].x;
	add_packet.y = clients[c_id].y;
	add_packet.z = clients[c_id].z;
	do_send(&add_packet);
}

void SESSION::send_move_packet(int c_id)
{
	SC_MOVE_PLAYER_PACKET p;
	p.id = c_id;
	p.size = sizeof(SC_MOVE_PLAYER_PACKET);
	p.type = SC_MOVE_PLAYER;
	p.x = clients[c_id].x;
	p.y = clients[c_id].y;
	p.z = clients[c_id].z;
	p.yaw = clients[c_id].player.GetYaw();
	p.pitch = clients[c_id].player.GetPitch();
	p.roll = clients[c_id].player.GetRoll();

	//std::cout << "[" << c_id << "]'s position : x = " << clients[c_id].x << ", y = " << clients[c_id].y << ", z = " << clients[c_id].z << std::endl;
	std::cout << "[" << c_id << "]'s look : yaw = " << p.yaw << ", pitch = " << p.pitch << ", roll = " << p.roll << std::endl;

	do_send(&p);
}
