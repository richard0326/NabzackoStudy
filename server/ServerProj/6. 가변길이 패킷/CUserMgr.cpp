#include "Global.h"
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

void CUserMgr::push_back(CSocket* sock)
{
	m_socketList.push_back(sock);
}

void CUserMgr::Release()
{
	list<CSocket*>::iterator iter;
	for (iter = m_socketList.begin(); iter != m_socketList.end();) {
		list<CSocket*>::iterator erase = iter;
		iter++;

		SAFE_DELETE(*erase)
		m_socketList.erase(erase);
	}
}