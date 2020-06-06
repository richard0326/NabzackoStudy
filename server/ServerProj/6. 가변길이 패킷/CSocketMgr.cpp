#include "Global.h"
#include "CSocket.h"
#include "CSocketMgr.h"
#include "CUserMgr.h"

const char* loginMsg[] = { "���̵� �ǹٸ��� �ʽ��ϴ�. �ٽ� �Է����ּ���.", "��й�ȣ�� Ʋ�Ƚ��ϴ�. �ٽ� �Է����ּ���.", "�α��� ����!" };
const char* resultMsg[] = { "���̵� �Է� ����", "��й�ȣ �Է� ����", "�α��� ����" };

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
	CSocket* sock = new CSocket();
	if (sock->Init(m_listen_sock) == false)
	{
		SINGLETON(CLogMgr)->Addlog("User ���� ����");
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

// ����� ���� ������ ���� �Լ�
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
	// �������� ��Ŷ
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

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI CSocketMgr::ProcessClient(LPVOID arg)
{
	CSocket* sock = (CSocket*)arg;
	SOCKET client_sock = sock->getSocket();
	int retval;
	SOCKADDR_IN clientaddr = sock->getAddr();

	int addrlen;
	char buf[BUFSIZE];

	// ��ü �ݺ���
	while (1)
	{
		int val1 = 0;
		int val2 = 0;
		IdInfo checkId;
		ZeroMemory(&checkId, sizeof(IdInfo));
		// ������ �ޱ�
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
		printf("������ID : %d\n��� : %s\n", threadId, resultMsg[logValue]);

		val2 = logValue;
		if (SendPacket(&val1, &val2, (char*)resultMsg[logValue], client_sock, 0) == false)
		{
			break;
		}

		if (logValue == LOGIN_SUCCESS)
		{
			printf("�α��� ����\n");
			break;
		}
		
	}
	return 0;
}