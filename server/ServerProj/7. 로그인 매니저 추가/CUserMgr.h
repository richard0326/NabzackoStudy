#pragma once

class CUser;
class CUserMgr {
	DECLARE_SINGLETON(CUserMgr)

private:
	CUserMgr();
	~CUserMgr();

public:
	bool Init();

	void push_back(CUser*);
	
private:
	void Release();

	list<CUser*> m_socketList;
};