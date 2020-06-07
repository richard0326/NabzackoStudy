#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

#define ID_ERROR		0
#define PW_ERROR		1
#define LOGIN_SUCCESS	2

// 고정 길이 데이터 패킷 - 유저 정보 구조체
struct IdInfo
{
	char ID[20];
	char PW[20];
};

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

bool RecvPacket(int& val1, int& val2, int& recvLen, char* recvMsg, SOCKET s)
{
	int total = 0;
	int retval = recvn(s, (char*)&total, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		printf("Recv total Error()\n");
		return false;
	}
	else if (retval == 0)
		return false;

	char buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	retval = recvn(s, buf, total - sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		printf("Recv Other Error()\n");
		return false;
	}
	else if (retval == 0)
		return false;

	memcpy(&val1, buf, sizeof(int));
	memcpy(&val2, buf + sizeof(int), sizeof(int));
	memcpy(&recvLen, buf + sizeof(int) + sizeof(int), sizeof(int));
	memcpy(recvMsg, buf + sizeof(int) + sizeof(int) + sizeof(int), recvLen);

	return true;
}

bool SendPacket(int val1, int val2, int msgLen, char* sendMsg, SOCKET s)
{
	// 가변길이 패킷
	int infoSize = msgLen;
	int total = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) + infoSize;

	char buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	memcpy(buf, &total, sizeof(int));
	memcpy(buf + sizeof(int), &val1, sizeof(int));
	memcpy(buf + sizeof(int) + sizeof(int), &val2, sizeof(int));
	memcpy(buf + sizeof(int) + sizeof(int) + sizeof(int), &msgLen, sizeof(int));
	memcpy(buf + sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int), sendMsg, infoSize);

	int retval = send(s, buf, total, 0);
	if (retval == SOCKET_ERROR) {
		printf("Send Error\n");
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1] = "";

	// 서버와 데이터 통신
	while (1) {
		// 입력 받을 UserInfo 구조체에 값을 넣어준다.
		IdInfo user;
		ZeroMemory(&user, sizeof(user));
		printf("ID:");
		scanf("%s", user.ID);

		printf("PW:");
		scanf("%s", user.PW);

		// 데이터 보내기
		int val1 = 0;
		int val2 = 0;
		int len = 0;
		if (SendPacket(val1, val2, sizeof(IdInfo), (char*)&user, sock) == false)
			break;

		char msg[BUFSIZE] = "";
		if (RecvPacket(val1, val2, len, (char*)msg, sock) == false)
			break;

		// 문자열을 출력한다.
		printf("%s\n", msg);
		puts("");

		if (val2 == LOGIN_SUCCESS)
		{
			break;
		}
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}