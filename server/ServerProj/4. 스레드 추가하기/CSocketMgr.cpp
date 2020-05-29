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

	int retval;			// return value�� ���ڷ� return�Ǵ� ���� �ޱ� �������� ����� ����

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	// ���� ���̺귯�� dll�� �޸𸮿� �ø��� �Լ�
	{
		err_display("wsa Error");
		return false;
	}
	// socket()
	m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);		// ������ �����ϴ� �Լ�		
	if (m_listen_sock == INVALID_SOCKET) {
		err_display("socket()");	// ���� ���� ���� �� ���� ��� �� ����
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
		err_display("bind()");								// ���� ��� ���н� ���� ��� �� ���� 
		return false;
	}
	// listen()
	retval = listen(m_listen_sock, SOMAXCONN);			// Ŭ���̾�Ʈ�� ������ �� �ִ� �������� ������ش�.
	if (retval == SOCKET_ERROR) {
		err_display("listen()");	// ���� ��ȯ ���н� ���� ��� �� ����
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

	// ���� ����
	WSACleanup();
}

void CSocketMgr::loop()
{
	printf("=== ���� ���� ��������! ===");

	while (true)
	{
		if (Accept() == false)
			continue;

		/*
		// ������ ����
		// Ŭ���̾�Ʈ�� ������ ���
		while (true)
		{
			
			// recv()
			retval = recv(client_sock, buf, sizeof(LoginInfo), 0);		// Ŭ���̾�Ʈ �������� ���� �����͸� �޴´�.

			if (retval == SOCKET_ERROR) {			// �����Ϳ� ������ ������ ���� ��� �� Ż��
				err_display("recv()");
				break;
			}
			else if (retval == 0)					// �����Ͱ� 0�� ��� ��� ���Ḧ �ǹ��Ѵ�.
				break;

			// ���� �������� ũ�� ���
			LoginInfo login;
			ZeroMemory(&login, sizeof(LoginInfo));
			memcpy(login.ID, (void*)buf, sizeof(char) * 20);
			memcpy(login.PW, (void*)(buf + (sizeof(char) * 20)), sizeof(char) * 20);
			int len1 = strlen(login.ID);
			int len2 = strlen(login.PW);
			login.ID[len1 - 1] = NULL;
			login.PW[len2 - 1] = NULL;

			printf("���̵� : %s, ��й�ȣ : %s\n", login.ID, login.PW);

			bool isSuccess = false;
			for (int i = 0; i < 3; i++) {
				if (strcmp(confirmLogin[i].ID, login.ID) == 0 && strcmp(confirmLogin[i].PW, login.PW) == 0) {
					isSuccess = true;
					break;
				}
			}

			ZeroMemory(buf, sizeof(BUFSIZE) + 1);
			if (isSuccess == true) {
				printf("�α��� ����\n");
				strcpy_s(buf, "�α��� ����");
			}
			else {
				printf("�α��� ����\n");
				strcpy_s(buf, "�α��� ����");
			}

			retval = send(client_sock, buf, retval, 0);		// Ŭ���̾�Ʈ �������� �����͸� ����
			if (retval == SOCKET_ERROR) {			// �����Ϳ� ������ ������ ���� ��� �� Ż��
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


// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// ����� ���� ������ ���� �Լ�
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


// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
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
		IdInfo checkId;

		// ������ �ޱ�
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
		printf("������ID : %d\n��� : %s\n", threadId, resultMsg[logValue]);

		// �������� ��Ŷ
		int total = 0;
		int msglen = strlen(loginMsg[logValue]);
		total = sizeof(int) + sizeof(int) + sizeof(int) + msglen;
		ZeroMemory(buf, sizeof(buf));
		memcpy(buf, &total, sizeof(int));
		memcpy(buf + sizeof(int), &logValue, sizeof(int));
		memcpy(buf + sizeof(int) + sizeof(int), &msglen, sizeof(int));
		memcpy(buf + sizeof(int) + sizeof(int) + sizeof(int), loginMsg[logValue], msglen);

		// ������ ������
		retval = send(client_sock, (char*)&buf, total, 0);
		if (retval == SOCKET_ERROR) {
			printf("send()\n");
			break;
		}


		if (logValue == LOGIN_SUCCESS)
		{
			strcpy(client_packet->client_IdInfo.ID, checkId.ID);
			strcpy(client_packet->client_IdInfo.PW, checkId.PW);
			printf("������ID : %d\n���̵�, �н����� ����Ϸ�\n�����带 �����մϴ�.\n", threadId);
			client_packet->loginSuccess = true;
			delete client_packet;
			client_packet = NULL;
			break;
		}
	}

	return 0;
}