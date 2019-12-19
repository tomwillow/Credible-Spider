#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)

#include <iostream>
#include "Manager.h"

using namespace std;

int main()
{
	//_CrtSetBreakAlloc(937);
	{
		Manager manager(1, 0);
		manager.readIn(cin);
	}

	_CrtDumpMemoryLeaks();
	return 0;
}