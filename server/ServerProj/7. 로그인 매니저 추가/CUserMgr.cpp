#include "Global.h"
#include "CUser.h"
#include "CUserMgr.h"

CUserMgr::CUserMgr()
{

}

CUserMgr::~CUserMgr()
{
	Release();
}


bool CUserMgr::Init()
{
	return true;
}

void CUserMgr::push_back(CUser* sock)
{
	m_socketList.push_back(sock);
}

void CUserMgr::Release()
{
	list<CUser*>::iterator iter;
	for (iter = m_socketList.begin(); iter != m_socketList.end();) {
		list<CUser*>::iterator erase = iter;
		iter++;

		SAFE_DELETE(*erase)
		m_socketList.erase(erase);
	}
}