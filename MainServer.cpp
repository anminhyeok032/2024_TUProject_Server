#include "global.h"
#include "protocol.h"
#include "Session.h"
#include "EXP_Over.h"

// 현재 월드 서버 구현중
// 모든 클라이언트(MAX = 4) 가 서버 소켓을 열고 브로드캐스트로 접속한 뒤,
// 방장 클라이언트 접속해제 시 기존 연결되있던 클라이언트 확인후, 방장을 위임함.
// 방장을 위임받은 클라이언트는 기존 연결되있던 서버에서 정보를 받아 그대로 월드 서버 역할을 하게 할 예정

void worker()
{
	// TODO : GetQueuedCompletionStatus를 이용한 worker 스레드 작성
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

	// 시작할 때 AcceptEx 를 걸어줌
	AcceptEx(server, c_socket, a_over._send_buf, 0, addr_size + 16, addr_size + 16, 0, &a_over._over);

	// 내 cpu 코어 개수만큼의 스레드 생성
	int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> worker_threads;

	for (int i = 0; i < num_threads; ++i)
	{
		// TODO : worker 함수 작성
		worker_threads.emplace_back(worker, server);
	}
	for (auto& w : worker_threads)
	{
		w.join();
	}


	closesocket(server);
	WSACleanup();
}