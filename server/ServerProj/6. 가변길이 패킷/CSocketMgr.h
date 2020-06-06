#pragma once

class CSocketMgr {

	DECLARE_SINGLETON(CSocketMgr)

private:
	SOCKET m_listen_sock;

	CSocketMgr();
	~CSocketMgr();
public:

	bool Init();
	void loop();
	
private:
	void Release();
	bool Accept();

	static int recvn(SOCKET s, char* buf, int len, int flags);
	static bool recvPacket(int* val1, int* val2, char* ret, SOCKET s, int flags);
	static DWORD WINAPI ProcessClient(LPVOID arg);
};