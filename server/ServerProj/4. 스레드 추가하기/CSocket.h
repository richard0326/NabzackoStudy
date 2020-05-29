#pragma once

class CSocket {
	// ������ ��ſ� ����� ����
	SOCKET m_client_sock;				// ����� ����
	SOCKADDR_IN m_clientaddr;			// ����� ������ �ּ�
public:
	CSocket();
	~CSocket();

	bool Init(SOCKET);
	void Release();

	SOCKET getSocket() {
		return m_client_sock;
	}

	SOCKADDR_IN getAddr() {
		return m_clientaddr;
	}
};