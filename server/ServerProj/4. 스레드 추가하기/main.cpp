#include "global.h"
#include "CSocketMgr.h"

struct LoginInfo {
	char ID[20];
	char PW[20];
};

LoginInfo confirmLogin[3] = { {"aaa", "111"}, {"bbb", "222"}, {"ccc", "333"} };



int main(int argc, char* argv[])
{
	CSocketMgr* sockMgr = new CSocketMgr();

	if (sockMgr->Init() == false)
		return 1;

	sockMgr->loop();
		
	return 0;
}