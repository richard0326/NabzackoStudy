#pragma once

#define SERVERPORT	9000		// ������ ��Ʈ ��ȣ
#define BUFSIZE		512			// ������ ũ��

#define LOGIN_FAIL		0
#define LOGIN_PASSFAIL	1
#define LOGIN_SUCCESS	2

const char* loginMsg[] = { "���̵� �ǹٸ��� �ʽ��ϴ�. �ٽ� �Է����ּ���.", "��й�ȣ�� Ʋ�Ƚ��ϴ�. �ٽ� �Է����ּ���.", "�α��� ����!" };
const char* resultMsg[] = { "���̵� �Է� ����", "��й�ȣ �Է� ����", "�α��� ����" };

int g_Idcount = 3;

struct IdInfo
{
	char ID[10];
	char PW[10];
};

IdInfo g_IdInfos[] = {
	{ "kja0204", "1234" },
	{ "aaaa1234", "1234" },
	{ "bbbb1234", "1234" }
};