#include "Global.h"
#include "CUser.h"
#include "CLoginMgr.h"

CUser::CUser()
{

}

CUser::~CUser()
{
	Release();
}

bool CUser::Init(SOCKET sock)
{
	CPacket::Init(sock);

	ZeroMemory(m_ID, IDPW_LEN);
	ZeroMemory(m_PW, IDPW_LEN);

	return true;
}

void CUser::Release()
{

}

bool CUser::Recv_Message()
{
	int protocol1 = 0;
	int protocol2 = 0;
	int msgLen = 0;
	char buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);

	if (RecvPacket(protocol1, protocol2, msgLen, buf) == false)
		return false;

	char ID[20];
	char PW[20];
	ZeroMemory(ID, IDPW_LEN);
	ZeroMemory(PW, IDPW_LEN);

	int retval = 0;
	int proto1 = 0;
	int proto2 = 0;
	int len = 0;

	switch (protocol1)
	{
	case PROTO_LOGIN:
		memcpy(ID, buf, IDPW_LEN);
		memcpy(PW, buf + IDPW_LEN, IDPW_LEN);
		retval = SINGLETON(CLoginMgr)->checkLogin(ID, PW);

		len = strlen(SINGLETON(CLoginMgr)->getLoginMsg(retval));

		proto2 = retval;
		if (SendPacket(proto1, proto2, len, (char*)SINGLETON(CLoginMgr)->getLoginMsg(retval)) == false)
		{
			break;
		}

		if (retval == LOGIN_SUCCESS)
		{
			SINGLETON(CLogMgr)->Addlog("로그인 성공\n");
			break;
		}
		break;

	default:
		SINGLETON(CLogMgr)->Addlog("Uncorrect Protocol value");
		return false;
	}

	return true;
}