#include "Global.h"
#include <ctime>
#include "CLogMgr.h"

CLogMgr::CLogMgr()
	:m_target(0)
{
	m_filename[0] = NULL;
}


CLogMgr::~CLogMgr(void)
{
	Release();
}

// �α� â ����
bool CLogMgr::Init(unsigned int target, const char* fileName)
{
	m_target = target;

	if (target & LOG_FILE && fileName != NULL)
	{
		strcpy_s(m_filename, MAX_PATH, fileName);
	}
	else
	{
		m_filename[0] = NULL;
	}
	
	return true;
}

// ����
void CLogMgr::Release(void)
{
}

// �α� ���
int CLogMgr::Addlog(const char* strmsg, ...)
{
	char	szBuff[1024];
	char	szDate[128];
	char	szTime[128];

	_strdate_s(szDate);
	_strtime_s(szTime);

	va_list ap;
	va_start(ap, strmsg);

	vsprintf_s(szBuff, strmsg, ap);

	va_end(ap);

	// �ܼ� ����� ���
	if (m_target & LOG_CONSOLE)
	{
		printf("(date[%s] time[%s]) : %s\n", szDate, szTime, szBuff);
	}

	// file ����� ���
	if (m_target & LOG_FILE)
	{
		ErrorFile("(date[%s] time[%s]) : %s\n", szDate, szTime, szBuff);
	}

	return 1;
}

// ���â�� ���� ���
void CLogMgr::ErrorOutput(const char* strmsg, ...)
{
	char szBuff[256];

	va_list		ap;

	va_start(ap, strmsg);

	vsprintf_s(szBuff, strmsg, ap);

	va_end(ap);

	OutputDebugString(szBuff);
	OutputDebugString("\n");
}


// �޼��� �ڽ��� ���� ���
void CLogMgr::ErrorMsgBox(const char* strmsg, ...)
{
	char szBuff[256];

	va_list		ap;

	va_start(ap, strmsg);

	vsprintf_s(szBuff, strmsg, ap);

	va_end(ap);

#ifdef _DEBUG
	MessageBox(NULL, szBuff, "Error", MB_ICONERROR | MB_OK);
#else
	// release ����ϋ��� ���Ϸ� ����.
	errorFile(szBuff);
#endif // #ifdef _DEBUG

}

// ���Ϸ� ���� �޼��� ����
void CLogMgr::ErrorFile(const char* strmsg, ...)
{
	SYSTEMTIME		st;
	GetLocalTime(&st);
	char szfile[256];

	if (m_target & LOG_FILE && m_filename[0] != NULL)
	{
		wsprintf(szfile, "%s.txt", m_filename);
	}
	else
	{
		wsprintf(szfile, "ERROR_LOG_%d��%d��%d��.txt", st.wYear, st.wMonth, st.wDay);
	}

	// C�� ��Ÿ���� ���� �����
	FILE* fp = NULL;
	fopen_s(&fp, szfile, "a");

	// ����ó��
	if (fp == NULL)
	{
		ErrorMsgBox("�α� ���� ���� ����");
	}
	else // ������ ������ �̾���̱�
	{
		char szBuff[256];

		va_list		ap;

		va_start(ap, strmsg);

		vsprintf_s(szBuff, strmsg, ap);

		va_end(ap);

		fprintf(fp, "%s\n", szBuff);
	}

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
}