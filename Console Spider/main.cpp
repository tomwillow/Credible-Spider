#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)

#include <iostream>
#include <vector>
#include "Manager.h"

using namespace std;


int main()
{
	//_CrtSetBreakAlloc(937);
	vector<int> ret;
	for (int i=0;i<100;++i)
	{
		cout << i << endl;
		Manager manager(1, i);
		if (manager.AutoSolve())
			ret.push_back(i);
		//manager.Command("auto");
		//manager.readIn(cin);
	cout << "solved:" << ret.size() << endl;
	for (auto i : ret)
		cout << i << " ";
	}


	_CrtDumpMemoryLeaks();
	return 0;
}