#include "Global.h"
#include "CSocket.h"
#include "CSocketMgr.h"
#include "CUserMgr.h"

const char* loginMsg[] = { "아이디가 옳바르지 않습니다. 다시 입력해주세요.", "비밀번호가 틀렸습니다. 다시 입력해주세요.", "로그인 성공!" };
const char* resultMsg[] = { "아이디 입력 오류", "비밀번호 입력 오류", "로그인 성공" };

int g_Idcount = 3;

struct IdInfo
{
	char ID[10];
	char PW[10];
};

IdInfo g_IdInfos[] = {
	{ "kja0204", "1234" },
	{ "aaaa1234", "1234" },
	{ "bbbb1234", "1234" }
};

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
		SINGLETON(CLogMgr)->Addlog("WSA 버전 설정 실패");
		return false;
	}

	// socket()
	m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);		// 소켓을 생성하는 함수		
	if (m_listen_sock == INVALID_SOCKET) {
		SINGLETON(CLogMgr)->Addlog("Listen 소켓 설정 실패");	// 소켓 생성 실패 시 에러 출력 후 종료
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
		SINGLETON(CLogMgr)->Addlog("Bind 실패");								// 접속 허용 실패시 에러 출력 후 종료 
		return false;
	}

	// listen()
	retval = listen(m_listen_sock, SOMAXCONN);			// 클라이언트를 수용할 수 있는 소켓으로 만들어준다.
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Listen 상태 실패");	// 소켓 변환 실패시 에러 출력 후 종료
		return false;
	}

	return true;
}

void CSocketMgr::Release() 
{
	// closesocket()
	closesocket(m_listen_sock);

	// 원속 종료
	WSACleanup();
}

void CSocketMgr::loop()
{
	SINGLETON(CLogMgr)->Addlog("=== 서버 시작 ㄷㄷㄷㅈ! ===\n");

	while (true)
	{
		if (Accept() == false)
			continue;
	}
}

bool CSocketMgr::Accept()
{
	CSocket* sock = new CSocket();
	if (sock->Init(m_listen_sock) == false)
	{
		SINGLETON(CLogMgr)->Addlog("User 연결 실패");
		SAFE_DELETE(sock)
		return false;
	}

	HANDLE hThread = CreateThread(NULL, 0, ProcessClient,
		(LPVOID)sock, 0, NULL);
	if (hThread == NULL) { 
		SAFE_DELETE(sock);
	}
	else { CloseHandle(hThread); }

	SINGLETON(CUserMgr)->push_back(sock);

	return true;
}

// 사용자 정의 데이터 수신 함수
int CSocketMgr::recvn(SOCKET s, char* buf, int len, int flags)
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

bool CSocketMgr::recvPacket(int* val1, int* val2, char* ret, SOCKET s, int flags)
{
	int total = 0;
	int retval = recv(s, (char*)&total, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Recv total Error()\n");
		return false;
	}
	else if (retval == 0)
		return false;

	char buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	retval = recv(s, buf, total - sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Recv Other Error()\n");
		return false;
	}
	else if (retval == 0)
		return false;

	memcpy(val1, buf, sizeof(int));
	memcpy(val2, buf + sizeof(int), sizeof(int));
	memcpy(ret, buf + sizeof(int) + sizeof(int), total - sizeof(int) - sizeof(int) - sizeof(int));

	return true;
}

bool SendPacket(int* val1, int* val2, char* info, SOCKET s, int flags)
{
	// 가변길이 패킷
	int infoSize = strlen((char*)info);
	int total = sizeof(int) + sizeof(int) + sizeof(int) + infoSize;

	char buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	memcpy(buf, &total, sizeof(int));
	memcpy(buf + sizeof(int), val1, sizeof(int));
	memcpy(buf + sizeof(int) + sizeof(int), val2, sizeof(int));
	memcpy(buf + sizeof(int) + sizeof(int) + sizeof(int), info, infoSize);

	int retval = send(s, buf, total, 0);
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Send Error\n");
		return false;
	}

	return true;
}

// 클라이언트와 데이터 통신
DWORD WINAPI CSocketMgr::ProcessClient(LPVOID arg)
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
		int val1 = 0;
		int val2 = 0;
		IdInfo checkId;
		ZeroMemory(&checkId, sizeof(IdInfo));
		// 데이터 받기
		if(recvPacket(&val1, &val2, (char*)&checkId, client_sock, 0) == false) {
			break;
		}

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

		val2 = logValue;
		if (SendPacket(&val1, &val2, (char*)resultMsg[logValue], client_sock, 0) == false)
		{
			break;
		}

		if (logValue == LOGIN_SUCCESS)
		{
			printf("로그인 성공\n");
			break;
		}
		
	}
	return 0;
}