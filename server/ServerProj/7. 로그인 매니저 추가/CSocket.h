#pragma once

class CSocket {
protected:
	// ������ ��ſ� ����� ����
	SOCKET m_client_sock;				// ����� ����
	SOCKADDR_IN m_clientaddr;			// ����� ������ �ּ�
public:
	CSocket();
	~CSocket();

	bool Init(SOCKET);
protected:
	void Release();

public:
	SOCKET getSocket();

	SOCKADDR_IN getAddr();
};