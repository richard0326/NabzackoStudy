#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define BUFSIZE		512

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
	exit(true);
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
	int received;		// 받은 문자열의 크기
	char* ptr = buf;	// 받는 데이터의 주소
	int left = len;		// 남은 문자열의 길이

	while (left > 0) {
		received = recv(s, ptr, left, flags);	// 데이터를 전송하고 전송한 만큼의 값을 확인한다.
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)		// 모두 전송했을 경우 탈출
			break;
		left -= received;	// 받은 만큼 문자열의 크기를 줄임
		ptr += received;	// 받은 만큼 포인터를 앞으로 이동
	}

	return (len - left);		// 데이터를 모두 보내면 len값을 리턴, 일부 보내지 않았을 경우 len - left 값을 리턴한다.
}

int main(int argc, char* argv[])
{
	int retval;		// return value의 약어

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)			// 윈속 라이브러리 dll을 메모리에 올린다.
		return true;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);		// 소켓을 생성한다.
	if (sock == INVALID_SOCKET) err_quit("socket()");	// 실패시 프로그램 종료

	// connect()
	SOCKADDR_IN serveraddr;								// 서버 주소 - 소켓 주소 구조체
	ZeroMemory(&serveraddr, sizeof(serveraddr));		// 0 으로 초기화
	serveraddr.sin_family = AF_INET;					// IPv4로 등록
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);	// 서버 번호 등록
	serveraddr.sin_port = htons(SERVERPORT);			// 포트 번호 등록
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));		// 서버와 소켓을 연결. 만약 bind함수가 실행되지 않을 경우
																			// 임의의 지역 IP주소와 포트 번호를 설정해줌.
	if (retval == SOCKET_ERROR) err_quit("connect()");	// 연결 실패시 프로그램 종료

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	// 서버와 데이터 통신
	while (true)
	{
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)	// 입력을 받고 입력 실패시 탈출
			break;

		// '\n' 문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);	// 서버로 데이터를 보낸다.
		if (retval == SOCKET_ERROR) {				// 데이터 보내기를 실패할 경우 에러 출력
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(sock, buf, retval, 0);		// 서버로 부터 데이터를 받는 함수 호출
		if (retval == SOCKET_ERROR) {				// 받기를 실패할 경우 오류 출력
			err_display("recv()");
			break;
		}
		else if (retval == 0)						// 종료 코드를 받을 경우 탈출
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	// closesocket ()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}