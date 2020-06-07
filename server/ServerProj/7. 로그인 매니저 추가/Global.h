#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _UNICODE
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "Singleton.h"
#include "Define.h"

#include <list>

#include "CLogMgr.h"