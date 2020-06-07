#include "Global.h"
#include "CLoginMgr.h"

const char* loginMsg[] = { "아이디가 옳바르지 않습니다. 다시 입력해주세요.", "비밀번호가 틀렸습니다. 다시 입력해주세요.", "로그인 성공!" };
const char* resultMsg[] = { "아이디 입력 오류", "비밀번호 입력 오류", "로그인 성공" };

int g_Idcount = 3;

struct IdInfo
{
	char ID[10];
	char PW[10];
};

IdInfo g_IdInfos[] = {
	{ "kja0204", "1234" },
	{ "aaaa1234", "1234" },
	{ "bbbb1234", "1234" }
};


CLoginMgr::CLoginMgr()
{

}

CLoginMgr::~CLoginMgr()
{
	Release();
}

bool CLoginMgr::Init()
{
	return true;
}

void CLoginMgr::Release()
{

}

int CLoginMgr::checkLogin(char* _id, char* _pw)
{
	int logValue = -1;
	for (int i = 0; i < g_Idcount; i++)
	{
		if (strcmp(_id, g_IdInfos[i].ID) == 0)
		{
			if (strcmp(_pw, g_IdInfos[i].PW) == 0)
			{
				logValue = LOGIN_SUCCESS;
			}
			else
			{
				logValue = LOGIN_PASSFAIL;
			}
		}
	}

	if (logValue == -1)
	{
		logValue = LOGIN_FAIL;
	}

	return logValue;
}

const char* CLoginMgr::getLoginMsg(int idx) {
	return loginMsg[idx];
}