#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define BUFSIZE		512

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
	exit(true);
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

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;		// ���� ���ڿ��� ũ��
	char* ptr = buf;	// �޴� �������� �ּ�
	int left = len;		// ���� ���ڿ��� ����

	while (left > 0) {
		received = recv(s, ptr, left, flags);	// �����͸� �����ϰ� ������ ��ŭ�� ���� Ȯ���Ѵ�.
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)		// ��� �������� ��� Ż��
			break;
		left -= received;	// ���� ��ŭ ���ڿ��� ũ�⸦ ����
		ptr += received;	// ���� ��ŭ �����͸� ������ �̵�
	}

	return (len - left);		// �����͸� ��� ������ len���� ����, �Ϻ� ������ �ʾ��� ��� len - left ���� �����Ѵ�.
}

int main(int argc, char* argv[])
{
	int retval;		// return value�� ���

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)			// ���� ���̺귯�� dll�� �޸𸮿� �ø���.
		return true;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);		// ������ �����Ѵ�.
	if (sock == INVALID_SOCKET) err_quit("socket()");	// ���н� ���α׷� ����

	// connect()
	SOCKADDR_IN serveraddr;								// ���� �ּ� - ���� �ּ� ����ü
	ZeroMemory(&serveraddr, sizeof(serveraddr));		// 0 ���� �ʱ�ȭ
	serveraddr.sin_family = AF_INET;					// IPv4�� ���
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);	// ���� ��ȣ ���
	serveraddr.sin_port = htons(SERVERPORT);			// ��Ʈ ��ȣ ���
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));		// ������ ������ ����. ���� bind�Լ��� ������� ���� ���
																			// ������ ���� IP�ּҿ� ��Ʈ ��ȣ�� ��������.
	if (retval == SOCKET_ERROR) err_quit("connect()");	// ���� ���н� ���α׷� ����

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	// ������ ������ ���
	while (true)
	{
		// ������ �Է�
		printf("����(1) ����(2) ��(3) : ");
		int val = 0;
		if (fgets(buf, BUFSIZE, stdin) == NULL)	// �Է��� �ް� �Է� ���н� Ż��
			break;

		val = strtol(buf, NULL, 10);

		// ������ ������
		ZeroMemory(buf, sizeof(buf));
		memcpy(buf, &val, sizeof(int));
		retval = send(sock, buf, sizeof(int), 0);	// ������ �����͸� ������.
		if (retval == SOCKET_ERROR) {				// ������ �����⸦ ������ ��� ���� ���
			err_display("send()");
			break;
		}

		// ������ �ޱ�
		ZeroMemory(buf, sizeof(buf));
		retval = recvn(sock, buf, sizeof(int), 0);		// ������ ���� �����͸� �޴� �Լ� ȣ��
		if (retval == SOCKET_ERROR) {				// �ޱ⸦ ������ ��� ���� ���
			err_display("recv()");
			break;
		}
		else if (retval == 0)						// ���� �ڵ带 ���� ��� Ż��
			break;

		int length = 0;
		memcpy((void*)&length, buf, sizeof(int));
		ZeroMemory(buf, sizeof(buf));
		retval = recvn(sock, buf, length, 0);		// ������ ���� �����͸� �޴� �Լ� ȣ��
		if (retval == SOCKET_ERROR) {				// �ޱ⸦ ������ ��� ���� ���
			err_display("recv()");
			break;
		}
		else if (retval == 0)						// ���� �ڵ带 ���� ��� Ż��
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[���� ������] %s\n", buf);
	}

	// closesocket ()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}