#pragma once

// ���� �ʱ�ȭ �ϴ� ������ �ű��
#define SERVERPORT	9000		// ������ ��Ʈ ��ȣ
#define BUFSIZE		512			// ������ ũ��

// �α��� ����ϴ� ������ �ű��
#define LOGIN_FAIL		0
#define LOGIN_PASSFAIL	1
#define LOGIN_SUCCESS	2


#define SERVER_NAME		"���۲�'s Server"

// ���� ����
#define SAFE_DELETE(p)          if(p) { delete p; p = NULL; }
#define SAFE_DELETE_ARRAY(p)    if(p) { delete [] p; p = NULL; }