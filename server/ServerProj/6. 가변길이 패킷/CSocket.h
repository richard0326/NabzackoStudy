#pragma once

class CSocket {
	// ������ ��ſ� ����� ����
	SOCKET m_client_sock;				// ����� ����
	SOCKADDR_IN m_clientaddr;			// ����� ������ �ּ�
public:
	CSocket();
	~CSocket();

	bool Init(SOCKET);
private:
	void Release();

public:
	SOCKET getSocket();

	SOCKADDR_IN getAddr();
};