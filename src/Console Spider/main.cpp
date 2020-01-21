#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)

#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include "Manager.h"

using namespace std;

void testInput()
{
	Manager manager;
	manager.ReadIn(cin);
}

mutex m;


void autoSolve(int i, int& threadNum, vector<int>& seedSolved, vector<int>& seedUnsolved)
{
	string sep(50, '-');

	stringstream out;
	Manager manager;

	m.lock();
	threadNum++;
	out << "now seed = " << i << endl;

	manager.Command("new 4 " + to_string(i));
	m.unlock();

	bool solved = manager.Command("auto");
	m.lock();
	if (solved)
		seedSolved.push_back(i);
	else
		seedUnsolved.push_back(i);

	out << endl << sep << endl;
	out << "solved:" << seedSolved.size() << endl;

	out << "solved seed:";
	for (auto i : seedSolved)
		out << i << " ";

	out << endl;
	out << "unsolved seed:";
	for (auto i : seedUnsolved)
		out << i << " ";
	out << endl << sep << endl;

	threadNum--;
	m.unlock();
}

void autoSolve_multi()
{
	vector<int>seedSolved, seedUnsolved;
	int threadNum = 0;
	for (int i = 0; i < 100; ++i)
	{
		while (threadNum > 4)
			this_thread::sleep_for(10ms);

		thread t(autoSolve, i, ref(threadNum), ref(seedSolved), ref(seedUnsolved));
		t.detach();
	}

	while (seedSolved.size() + seedUnsolved.size() < 100)
		this_thread::sleep_for(10ms);

	auto& out = cout;
	out << "solved:" << seedSolved.size() << endl;

	out << "solved seed:";
	for (auto i : seedSolved)
		out << i << " ";

	out << endl;
	out << "unsolved seed:";
	for (auto i : seedUnsolved)
		out << i << " ";
}

int main()
{
	//_CrtSetBreakAlloc(35129);
	{
		int i = 0;
		int threadNum = 0;
		vector<int>seedSolved, seedUnsolved;
		autoSolve(i,threadNum, seedSolved, seedUnsolved);

		auto& out = cout;
		out << "solved:" << seedSolved.size() << endl;

		out << "solved seed:";
		for (auto i : seedSolved)
			out << i << " ";

		out << endl;
		out << "unsolved seed:";
		for (auto i : seedUnsolved)
			out << i << " ";
	}
	MessageBeep(0);

	_CrtDumpMemoryLeaks();
	return 0;
}