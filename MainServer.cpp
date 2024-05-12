#include "global.h"
#include "protocol.h"
#include "Session.h"
#include "EXP_Over.h"
#include "db.h"

// 현재 월드 서버 구현중
// 서버를 따로 둔 Dedicated Server 형태로 작성
// 멀티코어를 이용한 구현



// Global variables
std::array<SESSION, MAX_USER> clients;
HANDLE g_hiocp;

// ElapsedTime 계산을 위한 변수
auto lastUpdateTime = std::chrono::high_resolution_clock::now();

int GetNewClientId()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (clients[i].in_use == S_STATE::ST_FREE)
		{
			return i;
		}
	}
	return -1;
}

void ProcessPacket(int c_id, char* packet)
{
	// ElapsedTime
	auto currentTime = std::chrono::high_resolution_clock::now();
	float elapsed_time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastUpdateTime).count();
	float elapsed_time_insec = elapsed_time;
	if (elapsed_time_insec > 0.1f)	elapsed_time_insec = 0.1f;
	lastUpdateTime = currentTime;

	switch (packet[1]) 
	{
	case CS_LOGIN: 
	{
		CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
		strcpy_s(clients[c_id]._name, p->name);

		clients[c_id].in_use = S_STATE::ST_INGAME;
		clients[c_id].send_login_info_packet();
		std::cout << clients[c_id]._name << " login " << std::endl;
		std::cout << "[" << c_id << "]'s Coord : x = " << clients[c_id].x << ", y = " << clients[c_id].y << ", z = " << clients[c_id].z << std::endl;

		for (auto& player : clients) 
		{
			if (S_STATE::ST_INGAME != player.in_use) continue;
			if (player._id == c_id) continue;
			
			player.send_add_player_packet(c_id);
			clients[c_id].send_add_player_packet(player._id);
		}
		 
		
		break;
	}
	case CS_MOVE:
	{
		CS_MOVE_PACKET* p = reinterpret_cast<CS_MOVE_PACKET*>(packet);
		clients[c_id].player.SetYaw(p->player_yaw);

		clients[c_id].player.InputActionMove(p->direction, p->camera_yaw);
		
		XMFLOAT3 newPosition = clients[c_id].player.Update(elapsed_time_insec, XMFLOAT3(clients[c_id].x, clients[c_id].y, clients[c_id].z));
		clients[c_id].player.UpdateRotate(elapsed_time_insec);
		clients[c_id].player.OrientRotationToMove(elapsed_time_insec);

		clients[c_id].x = newPosition.x;
		clients[c_id].y = newPosition.y;
		clients[c_id].z = newPosition.z;
		
		
		
		for (auto& pl : clients)
		{
			if (S_STATE::ST_INGAME == pl.in_use)
			{
				pl.send_move_packet(c_id);
			}
		}
		clients[c_id].player.SetYaw(0.0f);
		clients[c_id].player.SetDirectionVector(XMFLOAT3(0.f, 0.f, 0.f));
		break;
	}
	}
}

void disconnect(int c_id)
{
	// remove player 패킷 전송
	for (auto& pl : clients) 
	{
		if (pl.in_use != S_STATE::ST_INGAME) continue;
		if (pl._id == c_id) continue;
		SC_REMOVE_PLAYER_PACKET p;
		p.id = c_id;
		p.size = sizeof(p);
		p.type = SC_REMOVE_PLAYER;
		pl.do_send(&p);
	}
	closesocket(clients[c_id]._socket);
	clients[c_id].in_use = S_STATE::ST_FREE;
}

void Initialize()
{
	for (int i = 0; i < clients.size(); ++i)
	{
		clients[i]._id = i;
	}
}

void worker(SOCKET server)
{
	while (true) 
	{
		DWORD num_bytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over = nullptr;
		// GetQueuedcompletionStautus 핸들 및 키값 오버랩 구조체, 무한대로 기다림
		BOOL ret = GetQueuedCompletionStatus(g_hiocp, &num_bytes, &key, &over, INFINITE);
		OVER_EXP* ex_over = reinterpret_cast<OVER_EXP*>(over);
		if (FALSE == ret) 
		{
			if (ex_over->_comp_type == OP_ACCEPT) 
			{
				std::cout << "Accept Error";
				exit(-1);
			}
			else 
			{
				std::cout << "GQCS Error on client[" << key << "]\n";
				disconnect(static_cast<int>(key));
				if (ex_over->_comp_type == OP_SEND) delete ex_over;
				continue;
			}
		}

		switch (ex_over->_comp_type) 
		{
		case OP_ACCEPT: 
		{
			// 전체를 락 걸어야 하지만, 하나의 오버랩을 이용하고 기존 accept가 끝나야 다시 돌기때문에 락을 걸지 않음

			// 현재 STATE가 FREE인 것을 찾아서 할당함
			int client_id = GetNewClientId();
			SOCKET c_socket = ex_over->_client_socket;
			if (client_id != -1) 
			{
				clients[client_id].in_use = S_STATE::ST_ALLOC;
				clients[client_id].x = 100;
				clients[client_id].y = 500;
				clients[client_id].z = 100;
				clients[client_id]._id = client_id;
				clients[client_id]._name[0] = 0;
				clients[client_id]._prev_remain = 0;
				clients[client_id]._socket = c_socket;
				clients[client_id].player.SetPlayerId(client_id);
				clients[client_id].player.SetLook(0, 0, 1.0f);
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket),
					g_hiocp, client_id, 0);
				clients[client_id].do_recv();
				
			}
			else 
			{
				std::cout << "Max user exceeded.\n";
				closesocket(c_socket);
			}
			c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			ZeroMemory(&ex_over->_over, sizeof(ex_over->_over));
			ex_over->_client_socket = c_socket;
			int addr_size = sizeof(SOCKADDR_IN);
			AcceptEx(server, c_socket, ex_over->_send_buf, 0, addr_size + 16, addr_size + 16, 0, &ex_over->_over);
			break;
		}
		case OP_RECV: 
		{
			int remain_data = num_bytes + clients[key]._prev_remain;
			char* p = ex_over->_send_buf;
			while (remain_data > 0) 
			{
				int packet_size = p[0];
				if (packet_size <= remain_data) 
				{
					ProcessPacket(static_cast<int>(key), p);
					p = p + packet_size;
					remain_data = remain_data - packet_size;
				}
				else break;
			}
			clients[key]._prev_remain = remain_data;
			if (remain_data > 0)
			{
				memcpy(ex_over->_send_buf, p, remain_data);
			}
			clients[key].do_recv();
			break;
		}
		case OP_SEND:
			delete ex_over;
			break;
		}
	}
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET server = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(server, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	listen(server, SOMAXCONN);

	SOCKADDR_IN cl_addr;
	int addr_size = sizeof(cl_addr);
	int client_id = 0;

	// CompletionPort에 HANDLE 및 소켓 등록
	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(server), g_hiocp, 9999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	OVER_EXP a_over;
	a_over._comp_type = OP_ACCEPT;
	a_over._client_socket = c_socket;


	// TODO: 데이터 베이스 연결 문제 해결 필요
	//// 데이터 소스 이름, 사용자 ID, 비밀번호를 입력하세요.
	//const wchar_t* dataSource = L"TUK_DB";
	//const wchar_t* userID = L"TUK_DB";
	//const wchar_t* password = L"1234";

	//if (CheckLogin(dataSource, userID, password)) {
	//	std::wcout << L"Login check successful." << std::endl;
	//}
	//else {
	//	std::wcout << L"Login check failed." << std::endl;
	//}


	// 시작할 때 AcceptEx 를 걸어줌
	AcceptEx(server, c_socket, a_over._send_buf, 0, addr_size + 16, addr_size + 16, 0, &a_over._over);

	// 내 cpu 코어 개수만큼의 스레드 생성
	int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> worker_threads;

	for (int i = 0; i < num_threads; ++i)
	{
		worker_threads.emplace_back(worker, server);
	}
	for (auto& w : worker_threads)
	{
		w.join();
	}


	closesocket(server);
	WSACleanup();
}