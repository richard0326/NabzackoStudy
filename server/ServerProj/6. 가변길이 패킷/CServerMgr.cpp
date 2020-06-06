#include "Global.h"
#include "CServerMgr.h"
#include "CSocketMgr.h"
#include "CUserMgr.h"

CServerMgr::CServerMgr()
	:m_hThread(NULL)
{

}

CServerMgr::~CServerMgr()
{
	Release();
}

bool CServerMgr::Init()
{
	// 여기서 다른 싱글톤을 초기화를 해준다.
	SetConsoleTitleA(SERVER_NAME);

	if (SINGLETON(CLogMgr)->Init(LOG_ALL, NULL) == false) 
	{
		return false;
	}

	if (SINGLETON(CUserMgr)->Init() == false)
	{
		return false;
	}

	if (SINGLETON(CSocketMgr)->Init() == false)
	{
		return false;
	}

	return true;
}

void CServerMgr::Update()
{
	m_hThread = CreateThread(NULL, 0, Loop, NULL, 0, NULL);
	if (m_hThread == NULL) {
		SINGLETON(CLogMgr)->Addlog("Accept Loop 생성 실패\n");
		return;
	}

	while (true)
	{
		char input = _getwch();
		if (input == 'q') {
			break;
		}
		printf("test\n");
	}
}

void CServerMgr::Release()
{
	CloseHandle(m_hThread);
	
	DESTROY_SINGLETON(CUserMgr)
	// Singleton 객체 삭제
	DESTROY_SINGLETON(CSocketMgr)
		
	DESTROY_SINGLETON(CLogMgr)
}

DWORD CServerMgr::Loop(LPVOID arg)
{
	SINGLETON(CSocketMgr)->loop();

	return 0;
}