#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT	9000		// ������ ��Ʈ ��ȣ
#define BUFSIZE		512			// ������ ũ��

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

		// Ŭ���̾�Ʈ�� ������ ���
		while (true)
		{
			// recv()
			retval = recv(client_sock, buf, BUFSIZE, 0);		// Ŭ���̾�Ʈ �������� ���� �����͸� �޴´�.

			if (retval == SOCKET_ERROR) {			// �����Ϳ� ������ ������ ���� ��� �� Ż��
				err_display("recv()");
				break;
			}
			else if (retval == 0)					// �����Ͱ� 0�� ��� ��� ���Ḧ �ǹ��Ѵ�.
				break;

			// ���� �������� ũ�� ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			// send()
			retval = send(client_sock, buf, retval, 0);		// Ŭ���̾�Ʈ �������� �����͸� ����
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