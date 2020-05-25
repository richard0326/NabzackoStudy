#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT	9000		// 서버의 포트 번호
#define BUFSIZE		512			// 버퍼의 크기

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

int main(int argc, char* argv[])
{
	int retval;			// return value의 약자로 return되는 값을 받기 전용으로 사용할 변수

	// 원속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	// 윈속 라이브러리 dll을 메모리에 올리는 함수
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);		// 소켓을 생성하는 함수		
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");	// 소켓 생성 실패 시 에러 출력 후 종료

	// bind()
	SOCKADDR_IN serveraddr;							// 서버의 주소 정보를 담을 변수
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// 0으로 모두 초기화
	serveraddr.sin_family = AF_INET;				// IPv4로 등록 AF_INET = IPv4, AF_INET6 = IPv6
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	// 내 IP주소를 입력
	serveraddr.sin_port = htons(SERVERPORT);		// 내가 원하는 포트 번호를 입력 번호는1024 ~49151이 적당하다.

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));		// 생성된 소켓의 접속을 허용하고 지역 내의 IP주소와 포트번호 결정한다.
	if (retval == SOCKET_ERROR) err_quit("bind()");								// 접속 허용 실패시 에러 출력 후 종료 

	// listen()
	retval = listen(listen_sock, SOMAXCONN);			// 클라이언트를 수용할 수 있는 소켓으로 만들어준다.
	if (retval == SOCKET_ERROR) err_quit("listen()");	// 소켓 변환 실패시 에러 출력 후 종료

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;				// 연결될 소켓
	SOCKADDR_IN clientaddr;			// 연결될 소켓의 주소
	int addrlen;					// 연결될 소켓 주소의 크기 / clientaddr의 크기(받아들일 용량을 알려줘야 함)
	char buf[BUFSIZE + 1];

	while (true)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);	// 큐에 대기 중인 클라이언트를 소켓으로 받는다.

		if (client_sock == INVALID_SOCKET) {				// 클라이언트를 소켓으로 받는데 실패할 경우 에러 출력 후 다시 진행
			err_display("accept()");
			continue;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		while (true)
		{
			// recv()
			retval = recv(client_sock, buf, BUFSIZE, 0);		// 클라이언트 소켓으로 부터 데이터를 받는다.

			if (retval == SOCKET_ERROR) {			// 데이터에 문제가 있을시 에러 출력 후 탈출
				err_display("recv()");
				break;
			}
			else if (retval == 0)					// 데이터가 0일 경우 통신 종료를 의미한다.
				break;

			// 받은 데이터의 크기 출력
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			// send()
			retval = send(client_sock, buf, retval, 0);		// 클라이언트 소켓으로 데이터를 전송
			if (retval == SOCKET_ERROR) {			// 데이터에 문제가 있을시 에러 출력 후 탈출
				err_display("send()");
				break;
			}
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// 원속 종료
	WSACleanup();
	return 0;
}