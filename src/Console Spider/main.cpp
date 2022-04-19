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
#include "ScopeTime.h"

using namespace std;

void testInput()
{
	Manager manager;
	manager.ReadIn(cin);
}

mutex m;

void autoSolve(int i,int all_num, int& threadNum,int suit, vector<int>& seedSolved, vector<int>& seedUnsolved)
{
	m.lock();
	threadNum++;
	m.unlock();

	Manager manager;
	manager.Command("new "+to_string(suit)+" " + to_string(i));
	bool solved = manager.Command("auto");

	m.lock();
	if (solved)
		seedSolved.push_back(i);
	else
		seedUnsolved.push_back(i);

	string title = to_string(seedSolved.size() + seedUnsolved.size()) + "/" + to_string(all_num);
	title += " solved:" + to_string(seedSolved.size());
	SetConsoleTitle(title.c_str());

	threadNum--;
	m.unlock();
}

void print_result(const vector<int> &seedSolved, const vector<int> &seedUnsolved)
{
	string sep(50, '-');
	auto& out = cout;

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
}

void autoSolve_multi_thread(int suit,int start,int end)
{
	const int thread_limited = 11;
	vector<thread> vec_t;
	vector<int>seedSolved, seedUnsolved;
	int threadNum = 0;
	for (int i = start; i < end; ++i)
	{

		while (threadNum > thread_limited)
			this_thread::sleep_for(10ms);

		thread t(autoSolve, i,end-start,ref(threadNum),suit, ref(seedSolved), ref(seedUnsolved));
		t.detach();
	}

	while (seedSolved.size() + seedUnsolved.size() < (end-start))
		this_thread::sleep_for(10ms);

	print_result(seedSolved, seedUnsolved);
}

void autoSolve_single_thread(int suit,int start, int end)
{
	int threadNum = 0;
	vector<int>seedSolved, seedUnsolved;
	for (int i=start;i<end;++i)
	{
		autoSolve(i,end-start,threadNum,suit, seedSolved, seedUnsolved);
	}
	print_result(seedSolved, seedUnsolved);
}

int main()
{
	//_CrtSetBreakAlloc(35129);
	ScopeTime st;
	//autoSolve_multi_thread(2,0, 10);
	autoSolve_single_thread(4,0, 100);

	cout << st.elapsed();
	MessageBeep(0);
	system("pause");

	_CrtDumpMemoryLeaks();
	return 0;
}