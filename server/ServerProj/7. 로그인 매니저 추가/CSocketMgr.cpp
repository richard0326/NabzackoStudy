#include "Global.h"
#include "CUser.h"
#include "CSocketMgr.h"
#include "CUserMgr.h"

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
	CUser* user = new CUser();
	if (user->Init(m_listen_sock) == false)
	{
		SINGLETON(CLogMgr)->Addlog("User 연결 실패");
		SAFE_DELETE(user)
		return false;
	}

	HANDLE hThread = CreateThread(NULL, 0, ProcessClient,
		(LPVOID)user, 0, NULL);
	if (hThread == NULL) { 
		SAFE_DELETE(user);
	}
	else { CloseHandle(hThread); }

	SINGLETON(CUserMgr)->push_back(user);

	return true;
}

// 클라이언트와 데이터 통신
DWORD WINAPI CSocketMgr::ProcessClient(LPVOID arg)
{
	CUser* user = (CUser*)arg;

	int addrlen;
	char buf[BUFSIZE];

	// 전체 반복문
	while (1)
	{
		if (user->Recv_Message() == false)
			break;
	}
	return 0;
}