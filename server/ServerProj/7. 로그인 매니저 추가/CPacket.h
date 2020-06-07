#pragma once
#include "CSocket.h"

class CPacket : CSocket {
public:
	CPacket();
	~CPacket();

	bool Init(SOCKET sock);

protected:
	void Release();

	int recvn(SOCKET s, char* buf, int len, int flags);
public:
	bool RecvPacket(int& val1, int& val2, int& recvLen, char* recvMsg);
	bool SendPacket(int val1, int val2, int msgLen, char* sendMsg);
};