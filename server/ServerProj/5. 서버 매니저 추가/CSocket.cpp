#include "Global.h"
#include "CSocket.h"

CSocket::CSocket()
	:m_client_sock(NULL)
	,m_clientaddr()
{

}

CSocket::~CSocket()
{
	Release();
}

bool CSocket::Init(SOCKET listen) {
	// accept()
	int addrlen = sizeof(m_clientaddr);
	m_client_sock = accept(listen, (SOCKADDR*)&m_clientaddr, &addrlen);	// 큐에 대기 중인 클라이언트를 소켓으로 받는다.

	if (m_client_sock == INVALID_SOCKET) {				// 클라이언트를 소켓으로 받는데 실패할 경우 에러 출력 후 다시 진행
		return false;
	}

	// 접속한 클라이언트 정보 출력
	printf("\n[%s] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		SERVER_NAME,
		inet_ntoa(m_clientaddr.sin_addr),
		ntohs(m_clientaddr.sin_port));

	return true;
}

void CSocket::Release() {
	// closesocket()
	closesocket(m_client_sock);
	printf("[%s] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		SERVER_NAME,
		inet_ntoa(m_clientaddr.sin_addr), ntohs(m_clientaddr.sin_port));
}

