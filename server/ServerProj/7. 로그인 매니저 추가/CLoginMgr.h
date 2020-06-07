#pragma once

class CLoginMgr {

	DECLARE_SINGLETON(CLoginMgr)

private:
	CLoginMgr();
	~CLoginMgr();

public:
	bool Init();

private:
	void Release();

public:
	int checkLogin(char* _id, char* _pw);
	const char* getLoginMsg(int idx);
};