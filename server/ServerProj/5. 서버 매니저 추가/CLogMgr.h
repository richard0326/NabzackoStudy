#pragma once

// 로그 출력 타입
#define LOG_NONE			0x00000000
#define LOG_CONSOLE			0x00000001
#define LOG_FILE			0x00000002
#define LOG_ALL				( LOG_CONSOLE | LOG_FILE )

class CLogMgr
{
private:
	CLogMgr(void);
	~CLogMgr(void);

	DECLARE_SINGLETON(CLogMgr)

public:

	// 로그 창 설정
	bool	Init(unsigned int target, const char* fileName = NULL);

	// 로그 찍기
	int		Addlog(const char* strmsg, ...);

private:
	// 출력창에 에러 출력
	void	ErrorOutput(const char* strmsg, ...);

	// 메세지 박스로 에러 출력
	void	ErrorMsgBox(const char* strmsg, ...);

	// 파일로 에러 메세지 저장
	void	ErrorFile(const char* strmsg, ...);

	// 정리
	void	Release(void);

private:
	unsigned int	m_target;
	char			m_filename[MAX_PATH];
};