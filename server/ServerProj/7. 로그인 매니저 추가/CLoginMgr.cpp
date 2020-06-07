#include "Global.h"
#include "CLoginMgr.h"

const char* loginMsg[] = { "���̵� �ǹٸ��� �ʽ��ϴ�. �ٽ� �Է����ּ���.", "��й�ȣ�� Ʋ�Ƚ��ϴ�. �ٽ� �Է����ּ���.", "�α��� ����!" };
const char* resultMsg[] = { "���̵� �Է� ����", "��й�ȣ �Է� ����", "�α��� ����" };

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