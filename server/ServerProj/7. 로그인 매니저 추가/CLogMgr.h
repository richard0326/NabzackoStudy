#pragma once

// �α� ��� Ÿ��
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

	// �α� â ����
	bool	Init(unsigned int target, const char* fileName = NULL);

	// �α� ���
	int		Addlog(const char* strmsg, ...);

private:
	// ���â�� ���� ���
	void	ErrorOutput(const char* strmsg, ...);

	// �޼��� �ڽ��� ���� ���
	void	ErrorMsgBox(const char* strmsg, ...);

	// ���Ϸ� ���� �޼��� ����
	void	ErrorFile(const char* strmsg, ...);

	// ����
	void	Release(void);

private:
	unsigned int	m_target;
	char			m_filename[MAX_PATH];
};