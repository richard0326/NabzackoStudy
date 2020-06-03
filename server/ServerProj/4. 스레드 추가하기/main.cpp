#include "global.h"
#include "CSocketMgr.h"

int main(int argc, char* argv[])
{
	CSocketMgr* sockMgr = new CSocketMgr();

	if (sockMgr->Init() == false)
		return 1;

	sockMgr->loop();
		
	return 0;
}