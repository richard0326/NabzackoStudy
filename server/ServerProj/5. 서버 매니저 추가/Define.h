#pragma once

// 소켓 초기화 하는 곳으로 옮기기
#define SERVERPORT	9000		// 서버의 포트 번호
#define BUFSIZE		512			// 버퍼의 크기

// 로그인 담당하는 곳으로 옮기기
#define LOGIN_FAIL		0
#define LOGIN_PASSFAIL	1
#define LOGIN_SUCCESS	2


#define SERVER_NAME		"납작꼬's Server"

// 안전 해제
#define SAFE_DELETE(p)          if(p) { delete p; p = NULL; }
#define SAFE_DELETE_ARRAY(p)    if(p) { delete [] p; p = NULL; }