#pragma once

#define SERVERPORT	9000		// 서버의 포트 번호
#define BUFSIZE		512			// 버퍼의 크기

#define LOGIN_FAIL		0
#define LOGIN_PASSFAIL	1
#define LOGIN_SUCCESS	2

const char* loginMsg[] = { "아이디가 옳바르지 않습니다. 다시 입력해주세요.", "비밀번호가 틀렸습니다. 다시 입력해주세요.", "로그인 성공!" };
const char* resultMsg[] = { "아이디 입력 오류", "비밀번호 입력 오류", "로그인 성공" };

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