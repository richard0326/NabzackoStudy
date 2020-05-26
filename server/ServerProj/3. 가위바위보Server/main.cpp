#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>>
#include <iostream>
#include <string>
using namespace std;

#define SERVERPORT	9000		// ������ ��Ʈ ��ȣ
#define BUFSIZE		512			// ������ ũ��

int RSP(int v1, int v2) {
	// ���� 1
	// ���� 2
	// �� 3

	if (v1 == 1) {
		if (v2 == 2) {
			return 1;
		}
		else if (v2 == 1) {
			return 0;
		}
		else {
			return -1;
		}
	}
	else if (v1 == 2) {
		if (v2 == 2) {
			return 0;
		}
		else if (v2 == 1) {
			return -1;
		}
		else {
			return 1;
		}
	}
	else {
		if (v2 == 2) {
			return -1;
		}
		else if (v2 == 1) {
			return 1;
		}
		else {
			return 0;
		}
	}

	// �������� �̱� 1
	// ������ �̱� -1
	// ��� 0

	return 0;
}

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char* argv[])
{
	int retval;			// return value�� ���ڷ� return�Ǵ� ���� �ޱ� �������� ����� ����

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	// ���� ���̺귯�� dll�� �޸𸮿� �ø��� �Լ�
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);		// ������ �����ϴ� �Լ�		
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");	// ���� ���� ���� �� ���� ��� �� ����

	// bind()
	SOCKADDR_IN serveraddr;							// ������ �ּ� ������ ���� ����
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// 0���� ��� �ʱ�ȭ
	serveraddr.sin_family = AF_INET;				// IPv4�� ��� AF_INET = IPv4, AF_INET6 = IPv6
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	// �� IP�ּҸ� �Է�
	serveraddr.sin_port = htons(SERVERPORT);		// ���� ���ϴ� ��Ʈ ��ȣ�� �Է� ��ȣ��1024 ~49151�� �����ϴ�.

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));		// ������ ������ ������ ����ϰ� ���� ���� IP�ּҿ� ��Ʈ��ȣ �����Ѵ�.
	if (retval == SOCKET_ERROR) err_quit("bind()");								// ���� ��� ���н� ���� ��� �� ���� 

	// listen()
	retval = listen(listen_sock, SOMAXCONN);			// Ŭ���̾�Ʈ�� ������ �� �ִ� �������� ������ش�.
	if (retval == SOCKET_ERROR) err_quit("listen()");	// ���� ��ȯ ���н� ���� ��� �� ����

	// ������ ��ſ� ����� ����
	SOCKET client_sock;				// ����� ����
	SOCKADDR_IN clientaddr;			// ����� ������ �ּ�
	int addrlen;					// ����� ���� �ּ��� ũ�� / clientaddr�� ũ��(�޾Ƶ��� �뷮�� �˷���� ��)
	char buf[BUFSIZE + 1];

	// ������ ��ſ� ����� ����
	SOCKET client_sock2;				// ����� ����
	SOCKADDR_IN clientaddr2;			// ����� ������ �ּ�

	printf("=== ���� ���� ===");

	while (true)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);	// ť�� ��� ���� Ŭ���̾�Ʈ�� �������� �޴´�.

		if (client_sock == INVALID_SOCKET) {				// Ŭ���̾�Ʈ�� �������� �޴µ� ������ ��� ���� ��� �� �ٽ� ����
			err_display("accept()");
			continue;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock2 = accept(listen_sock, (SOCKADDR*)&clientaddr2, &addrlen);	// ť�� ��� ���� Ŭ���̾�Ʈ�� �������� �޴´�.

		if (client_sock2 == INVALID_SOCKET) {				// Ŭ���̾�Ʈ�� �������� �޴µ� ������ ��� ���� ��� �� �ٽ� ����
			err_display("accept()");
			continue;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr2.sin_addr),
			ntohs(clientaddr2.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (true)
		{
			// recv()
			retval = recv(client_sock, buf, sizeof(int), 0);		// Ŭ���̾�Ʈ �������� ���� �����͸� �޴´�.

			if (retval == SOCKET_ERROR) {			// �����Ϳ� ������ ������ ���� ��� �� Ż��
				err_display("recv()");
				break;
			}
			else if (retval == 0)					// �����Ͱ� 0�� ��� ��� ���Ḧ �ǹ��Ѵ�.
				break;

			// ���� �������� ũ�� ���
			int val1 = 0;
			memcpy((void*)&val1, (void*)buf, sizeof(int));

			ZeroMemory(buf, sizeof(buf));

			// recv()
			retval = recv(client_sock2, buf, sizeof(int), 0);		// Ŭ���̾�Ʈ �������� ���� �����͸� �޴´�.

			if (retval == SOCKET_ERROR) {			// �����Ϳ� ������ ������ ���� ��� �� Ż��
				err_display("recv()");
				break;
			}
			else if (retval == 0)					// �����Ͱ� 0�� ��� ��� ���Ḧ �ǹ��Ѵ�.
				break;

			int val2 = 0;
			memcpy((void*)&val2, (void*)buf, sizeof(int));

			string ret1 = "���";
			string ret2 = "���";

			if (RSP(val1, val2) == 1)
			{
				ret1 = "��";
				ret2 = "�̱�";
			}
			else if (RSP(val1, val2) == -1) {
				ret1 = "�̱�";
				ret2 = "��";
			}

			ZeroMemory(buf, sizeof(buf));
			int len1 = ret1.length();
			memcpy(buf, (void*)&len1, sizeof(int));
			memcpy(buf+sizeof(int), ret1.c_str(), ret1.length());

			retval = send(client_sock, buf, sizeof(int)+ret1.length(), 0);		// Ŭ���̾�Ʈ �������� �����͸� ����
			if (retval == SOCKET_ERROR) {			// �����Ϳ� ������ ������ ���� ��� �� Ż��
				err_display("send()");
				break;
			}

			ZeroMemory(buf, sizeof(buf));
			int len2 = ret2.length();
			memcpy(buf, (void*)&len2, sizeof(int));
			memcpy(buf + sizeof(int), ret2.c_str(), ret2.length());

			retval = send(client_sock2, buf, sizeof(int) + ret2.length(), 0);		// Ŭ���̾�Ʈ �������� �����͸� ����
			if (retval == SOCKET_ERROR) {			// �����Ϳ� ������ ������ ���� ��� �� Ż��
				err_display("send()");
				break;
			}
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}