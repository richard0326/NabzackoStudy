#include "global.h"
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
	m_client_sock = accept(listen, (SOCKADDR*)&m_clientaddr, &addrlen);	// ť�� ��� ���� Ŭ���̾�Ʈ�� �������� �޴´�.

	if (m_client_sock == INVALID_SOCKET) {				// Ŭ���̾�Ʈ�� �������� �޴µ� ������ ��� ���� ��� �� �ٽ� ����
		return false;
	}

	// ������ Ŭ���̾�Ʈ ���� ���
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(m_clientaddr.sin_addr),
		ntohs(m_clientaddr.sin_port));

	return true;
}

void CSocket::Release() {
	// closesocket()
	closesocket(m_client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(m_clientaddr.sin_addr), ntohs(m_clientaddr.sin_port));
}

