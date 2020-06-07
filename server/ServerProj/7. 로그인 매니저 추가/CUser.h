#pragma once
#include "CPacket.h"

#define IDPW_LEN 20
#define PROTO_LOGIN 0

class CUser : CPacket {
private:
	char m_ID[IDPW_LEN];
	char m_PW[IDPW_LEN];

public:
	CUser();
	~CUser();

	bool Init(SOCKET sock);

private:
	void Release();

public:
	bool Recv_Message();
};