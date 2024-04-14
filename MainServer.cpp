#include "global.h"
#include "protocol.h"
#include "Session.h"
#include "EXP_Over.h"

// ���� ���� ���� ������
// ��� Ŭ���̾�Ʈ(MAX = 4) �� ���� ������ ���� ��ε�ĳ��Ʈ�� ������ ��,
// ���� Ŭ���̾�Ʈ �������� �� ���� ������ִ� Ŭ���̾�Ʈ Ȯ����, ������ ������.
// ������ ���ӹ��� Ŭ���̾�Ʈ�� ���� ������ִ� �������� ������ �޾� �״�� ���� ���� ������ �ϰ� �� ����

void worker()
{
	// TODO : GetQueuedCompletionStatus�� �̿��� worker ������ �ۼ�
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

	// CompletionPort�� HANDLE �� ���� ���
	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(server), g_hiocp, 9999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	OVER_EXP a_over;
	a_over._comp_type = OP_ACCEPT;
	a_over._client_socket = c_socket;

	// ������ �� AcceptEx �� �ɾ���
	AcceptEx(server, c_socket, a_over._send_buf, 0, addr_size + 16, addr_size + 16, 0, &a_over._over);

	// �� cpu �ھ� ������ŭ�� ������ ����
	int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> worker_threads;

	for (int i = 0; i < num_threads; ++i)
	{
		// TODO : worker �Լ� �ۼ�
		worker_threads.emplace_back(worker, server);
	}
	for (auto& w : worker_threads)
	{
		w.join();
	}


	closesocket(server);
	WSACleanup();
}