#pragma once

class CSocket;
class CSocketMgr {

private:
	SOCKET m_listen_sock;
	list<CSocket*> m_socketList;
public:
	CSocketMgr();
	~CSocketMgr();

	bool Init();
	void Release();
	void loop();
	
private:
	bool Accept();
	void err_quit(const char* msg);
	void err_display(const char* msg);
};