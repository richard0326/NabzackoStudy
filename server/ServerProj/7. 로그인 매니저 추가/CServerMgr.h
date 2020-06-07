#pragma once

class CServerMgr
{
private:
	CServerMgr();
	~CServerMgr();

	DECLARE_SINGLETON(CServerMgr);
public:
	bool Init();
	void Update();
private:
	void Release();
	static DWORD Loop(LPVOID arg);

private:
	HANDLE m_hThread;
};