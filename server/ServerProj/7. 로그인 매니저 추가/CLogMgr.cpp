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

// 로그 창 설정
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

// 정리
void CLogMgr::Release(void)
{
}

// 로그 찍기
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

	// 콘솔 출력일 경우
	if (m_target & LOG_CONSOLE)
	{
		printf("(date[%s] time[%s]) : %s\n", szDate, szTime, szBuff);
	}

	// file 출력일 경우
	if (m_target & LOG_FILE)
	{
		ErrorFile("(date[%s] time[%s]) : %s\n", szDate, szTime, szBuff);
	}

	return 1;
}

// 출력창에 에러 출력
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


// 메세지 박스로 에러 출력
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
	// release 모드일떄는 파일로 쓴다.
	errorFile(szBuff);
#endif // #ifdef _DEBUG

}

// 파일로 에러 메세지 저장
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
		wsprintf(szfile, "ERROR_LOG_%d년%d월%d일.txt", st.wYear, st.wMonth, st.wDay);
	}

	// C형 스타일의 파일 입출력
	FILE* fp = NULL;
	fopen_s(&fp, szfile, "a");

	// 에러처리
	if (fp == NULL)
	{
		ErrorMsgBox("로그 파일 저장 실패");
	}
	else // 파일이 있으면 이어붙이기
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