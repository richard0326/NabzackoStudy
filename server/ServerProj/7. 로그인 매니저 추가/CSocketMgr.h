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

	static DWORD WINAPI ProcessClient(LPVOID arg);
};