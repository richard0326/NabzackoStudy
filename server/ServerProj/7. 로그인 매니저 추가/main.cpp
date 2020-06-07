#include "Global.h"
#include "CServerMgr.h"

int main(int argc, char* argv[])
{
	if (SINGLETON(CServerMgr)->Init() == false) {
		DESTROY_SINGLETON(CServerMgr);
		return 1;
	}

	SINGLETON(CServerMgr)->Update();
	DESTROY_SINGLETON(CServerMgr);
	return 0;
}