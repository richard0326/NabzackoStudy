#pragma once

class CSocket;
class CUserMgr {
	DECLARE_SINGLETON(CUserMgr)

private:
	CUserMgr();
	~CUserMgr();

public:
	bool Init();

	void push_back(CSocket*);
	
private:
	void Release();

	list<CSocket*> m_socketList;
};