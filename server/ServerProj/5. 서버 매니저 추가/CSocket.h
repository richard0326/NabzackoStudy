#pragma once

class CSocket {
	// 데이터 통신에 사용할 변수
	SOCKET m_client_sock;				// 연결될 소켓
	SOCKADDR_IN m_clientaddr;			// 연결될 소켓의 주소
public:
	CSocket();
	~CSocket();

	bool Init(SOCKET);
private:
	void Release();

public:
	SOCKET getSocket() {
		return m_client_sock;
	}

	SOCKADDR_IN getAddr() {
		return m_clientaddr;
	}
};