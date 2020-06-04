#pragma once

class CSocket;
class CSocketMgr {

	DECLARE_SINGLETON(CSocketMgr)

private:
	SOCKET m_listen_sock;
	list<CSocket*> m_socketList;

	CSocketMgr();
	~CSocketMgr();
public:

	bool Init();
	void loop();
	
private:
	void Release();
	bool Accept();

	static DWORD WINAPI ProcessClient(LPVOID arg);
};