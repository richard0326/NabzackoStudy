#include "global.h"
#include "CSocketMgr.h"
#include "CSocket.h"

DWORD WINAPI ProcessClient(LPVOID arg);

CSocketMgr::CSocketMgr() 
	: m_listen_sock(NULL)
{
	
}

CSocketMgr::~CSocketMgr() {
	Release();
}

bool CSocketMgr::Init() {

	int retval;			// return value의 약자로 return되는 값을 받기 전용으로 사용할 변수

	// 원속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	// 윈속 라이브러리 dll을 메모리에 올리는 함수
	{
		err_display("wsa Error");
		return false;
	}
	// socket()
	m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);		// 소켓을 생성하는 함수		
	if (m_listen_sock == INVALID_SOCKET) {
		err_display("socket()");	// 소켓 생성 실패 시 에러 출력 후 종료
		return false;
	}
	// bind()
	SOCKADDR_IN serveraddr;							// 서버의 주소 정보를 담을 변수
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// 0으로 모두 초기화
	serveraddr.sin_family = AF_INET;				// IPv4로 등록 AF_INET = IPv4, AF_INET6 = IPv6
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	// 내 IP주소를 입력
	serveraddr.sin_port = htons(SERVERPORT);		// 내가 원하는 포트 번호를 입력 번호는1024 ~49151이 적당하다.

	retval = bind(m_listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));		// 생성된 소켓의 접속을 허용하고 지역 내의 IP주소와 포트번호 결정한다.
	if (retval == SOCKET_ERROR) {
		err_display("bind()");								// 접속 허용 실패시 에러 출력 후 종료 
		return false;
	}
	// listen()
	retval = listen(m_listen_sock, SOMAXCONN);			// 클라이언트를 수용할 수 있는 소켓으로 만들어준다.
	if (retval == SOCKET_ERROR) {
		err_display("listen()");	// 소켓 변환 실패시 에러 출력 후 종료
		return false;
	}
	return true;
}

void CSocketMgr::Release() {
	list<CSocket*>::iterator iter;
	for (iter = m_socketList.begin(); iter != m_socketList.end(); iter++) {
		delete(*iter);
		m_socketList.erase(iter);
	}

	// closesocket()
	closesocket(m_listen_sock);

	// 원속 종료
	WSACleanup();
}

void CSocketMgr::loop()
{
	printf("=== 서버 시작 ㄷㄷㄷㅈ! ===");

	while (true)
	{
		if (Accept() == false)
			continue;

		/*
		// 쓰레드 생성
		// 클라이언트와 데이터 통신
		while (true)
		{
			
			// recv()
			retval = recv(client_sock, buf, sizeof(LoginInfo), 0);		// 클라이언트 소켓으로 부터 데이터를 받는다.

			if (retval == SOCKET_ERROR) {			// 데이터에 문제가 있을시 에러 출력 후 탈출
				err_display("recv()");
				break;
			}
			else if (retval == 0)					// 데이터가 0일 경우 통신 종료를 의미한다.
				break;

			// 받은 데이터의 크기 출력
			LoginInfo login;
			ZeroMemory(&login, sizeof(LoginInfo));
			memcpy(login.ID, (void*)buf, sizeof(char) * 20);
			memcpy(login.PW, (void*)(buf + (sizeof(char) * 20)), sizeof(char) * 20);
			int len1 = strlen(login.ID);
			int len2 = strlen(login.PW);
			login.ID[len1 - 1] = NULL;
			login.PW[len2 - 1] = NULL;

			printf("아이디 : %s, 비밀번호 : %s\n", login.ID, login.PW);

			bool isSuccess = false;
			for (int i = 0; i < 3; i++) {
				if (strcmp(confirmLogin[i].ID, login.ID) == 0 && strcmp(confirmLogin[i].PW, login.PW) == 0) {
					isSuccess = true;
					break;
				}
			}

			ZeroMemory(buf, sizeof(BUFSIZE) + 1);
			if (isSuccess == true) {
				printf("로그인 성공\n");
				strcpy_s(buf, "로그인 성공");
			}
			else {
				printf("로그인 실패\n");
				strcpy_s(buf, "로그인 실패");
			}

			retval = send(client_sock, buf, retval, 0);		// 클라이언트 소켓으로 데이터를 전송
			if (retval == SOCKET_ERROR) {			// 데이터에 문제가 있을시 에러 출력 후 탈출
				err_display("send()");
				break;
			}
		}
		*/
		cout << "test" << endl;
		//
	}
}

bool CSocketMgr::Accept()
{
	CSocket* sock = new CSocket();
	if (sock->Init(m_listen_sock) == false)
	{
		err_display("accept()");
		delete(sock);
		return false;
	}

	HANDLE hThread = CreateThread(NULL, 0, ProcessClient,
		(LPVOID)sock, 0, NULL);
	if (hThread == NULL) { 
		delete(sock);
	}
	else { CloseHandle(hThread); }

	m_socketList.push_back(sock);

	return true;
}


// 소켓 함수 오류 출력 후 종료
void CSocketMgr::err_quit(const char* msg)
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
void CSocketMgr::err_display(const char* msg)
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


// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	CSocket* sock = (CSocket*)arg;
	SOCKET client_sock = sock->getSocket();
	int retval;
	SOCKADDR_IN clientaddr = sock->getAddr();

	int addrlen;
	char buf[BUFSIZE];

	// 전체 반복문
	while (1)
	{
		IdInfo checkId;

		// 데이터 받기
		retval = recvn(client_sock, (char*)&checkId, sizeof(IdInfo), 0);
		if (retval == SOCKET_ERROR) {
			printf("recvn()\n");
			break;
		}
		else if (retval == 0)
			break;

		int logValue = -1;
		for (int i = 0; i < g_Idcount; i++)
		{
			if (strcmp(checkId.ID, g_IdInfos[i].ID) == 0)
			{
				if (strcmp(checkId.PW, g_IdInfos[i].PW) == 0)
				{
					logValue = LOGIN_SUCCESS;
				}
				else
				{
					logValue = LOGIN_PASSFAIL;
				}
			}
		}

		if (logValue == -1)
		{
			logValue = LOGIN_FAIL;
		}

		DWORD threadId = GetCurrentThreadId();
		printf("스레드ID : %d\n결과 : %s\n", threadId, resultMsg[logValue]);

		// 가변길이 패킷
		int total = 0;
		int msglen = strlen(loginMsg[logValue]);
		total = sizeof(int) + sizeof(int) + sizeof(int) + msglen;
		ZeroMemory(buf, sizeof(buf));
		memcpy(buf, &total, sizeof(int));
		memcpy(buf + sizeof(int), &logValue, sizeof(int));
		memcpy(buf + sizeof(int) + sizeof(int), &msglen, sizeof(int));
		memcpy(buf + sizeof(int) + sizeof(int) + sizeof(int), loginMsg[logValue], msglen);

		// 데이터 보내기
		retval = send(client_sock, (char*)&buf, total, 0);
		if (retval == SOCKET_ERROR) {
			printf("send()\n");
			break;
		}


		if (logValue == LOGIN_SUCCESS)
		{
			strcpy(client_packet->client_IdInfo.ID, checkId.ID);
			strcpy(client_packet->client_IdInfo.PW, checkId.PW);
			printf("스레드ID : %d\n아이디, 패스워드 저장완료\n쓰레드를 종료합니다.\n", threadId);
			client_packet->loginSuccess = true;
			delete client_packet;
			client_packet = NULL;
			break;
		}
	}

	return 0;
}