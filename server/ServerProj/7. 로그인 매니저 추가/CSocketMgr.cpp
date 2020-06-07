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

	int retval;			// return value�� ���ڷ� return�Ǵ� ���� �ޱ� �������� ����� ����

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	// ���� ���̺귯�� dll�� �޸𸮿� �ø��� �Լ�
	{
		SINGLETON(CLogMgr)->Addlog("WSA ���� ���� ����");
		return false;
	}

	// socket()
	m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);		// ������ �����ϴ� �Լ�		
	if (m_listen_sock == INVALID_SOCKET) {
		SINGLETON(CLogMgr)->Addlog("Listen ���� ���� ����");	// ���� ���� ���� �� ���� ��� �� ����
		return false;
	}

	// bind()
	SOCKADDR_IN serveraddr;							// ������ �ּ� ������ ���� ����
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// 0���� ��� �ʱ�ȭ
	serveraddr.sin_family = AF_INET;				// IPv4�� ��� AF_INET = IPv4, AF_INET6 = IPv6
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	// �� IP�ּҸ� �Է�
	serveraddr.sin_port = htons(SERVERPORT);		// ���� ���ϴ� ��Ʈ ��ȣ�� �Է� ��ȣ��1024 ~49151�� �����ϴ�.

	retval = bind(m_listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));		// ������ ������ ������ ����ϰ� ���� ���� IP�ּҿ� ��Ʈ��ȣ �����Ѵ�.
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Bind ����");								// ���� ��� ���н� ���� ��� �� ���� 
		return false;
	}

	// listen()
	retval = listen(m_listen_sock, SOMAXCONN);			// Ŭ���̾�Ʈ�� ������ �� �ִ� �������� ������ش�.
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Listen ���� ����");	// ���� ��ȯ ���н� ���� ��� �� ����
		return false;
	}

	return true;
}

void CSocketMgr::Release() 
{
	// closesocket()
	closesocket(m_listen_sock);

	// ���� ����
	WSACleanup();
}

void CSocketMgr::loop()
{
	SINGLETON(CLogMgr)->Addlog("=== ���� ���� ��������! ===\n");

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
		SINGLETON(CLogMgr)->Addlog("User ���� ����");
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

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI CSocketMgr::ProcessClient(LPVOID arg)
{
	CUser* user = (CUser*)arg;

	int addrlen;
	char buf[BUFSIZE];

	// ��ü �ݺ���
	while (1)
	{
		if (user->Recv_Message() == false)
			break;
	}
	return 0;
}