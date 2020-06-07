#include "Global.h"
#include "CPacket.h"

CPacket::CPacket()
{

}

CPacket::~CPacket()
{
	Release();
}

bool CPacket::Init(SOCKET sock)
{
	if (CSocket::Init(sock) == false)
		return false;


	return true;
}

void CPacket::Release()
{

}

// 사용자 정의 데이터 수신 함수
int CPacket::recvn(SOCKET s, char* buf, int len, int flags)
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

bool CPacket::RecvPacket(int& val1, int& val2, int& recvLen, char* recvMsg)
{
	int total = 0;
	int retval = recvn(m_client_sock, (char*)&total, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Recv total Error()\n");
		return false;
	}
	else if (retval == 0)
		return false;

	char buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	retval = recvn(m_client_sock, buf, total - sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Recv Other Error()\n");
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

bool CPacket::SendPacket(int val1, int val2, int msgLen, char* sendMsg)
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

	int retval = send(m_client_sock, buf, total, 0);
	if (retval == SOCKET_ERROR) {
		SINGLETON(CLogMgr)->Addlog("Send Error\n");
		return false;
	}

	return true;
}