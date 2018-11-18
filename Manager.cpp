#include "Manager.h"

#include "Poker\Add13.h"
#include "Poker\Deal.h"
#include "Poker\PMove.h"
#include "Poker\ReleaseCorner.h"

using namespace std;

Manager::Manager()
{
	poker = new Poker;
}


Manager::~Manager()
{
	delete poker;
	ReleaseRecord();
}

void Manager::ReleaseRecord()
{
	for (auto &r : record)
		delete r;
	record.clear();
}

void Manager::testMove(Poker *poker)
{
	int orig, start, dest;
	cout << "--testMove--" << endl << "input orig, start, dest: "; cin >> orig >> start >> dest;
	cout << endl;
	cout << "Chose: "; poker->printCard(orig, start); cout << endl;
	cout << "canMove? ";
	Action *action = new PMove(orig, start, dest);
	action->Do(poker);
	cout << ((PMove *)action)->success;
	cout << endl;

	record.push_back(action);
}

void Manager::showHelpInfo() const
{
	cout << "--Command Line Help--" << endl;
	cout << "a add13" << endl;
	cout << "d deal with seed" << endl;
	cout << "dr deal randomly" << endl;
	cout << "h help" << endl;
	cout << "m move" << endl;
	cout << "p print" << endl;
	cout << "r releaseCorner" << endl;
	cout << "redo" << endl;
	cout << "--Help End--" << endl << endl;
}

void Manager::test()
{
	showHelpInfo();
	string command;
	//cout << ">>";
	while ((cout<<">>")&& (cin >> command))
	{
		if (command == "act")
		{
			Act();
			continue;
		}
		if (command == "a")
		{
			int deskNum;
			cout << "--add13--" << endl << "input deskNum: "; cin >> deskNum;
			cout << endl;
			Action *action = new Add13(deskNum);
			action->Do(poker);
			record.push_back(action);

			poker->printCard();
			continue;
		}
		if (command == "d")
		{
			ReleaseRecord();

			int suitNum, seed;
			cout << "--deal with seed--" << endl << "input suitNum, seed: "; cin >> suitNum >> seed;
			cout << endl;
			Action *action = new Deal(suitNum, seed);
			action->Do(poker);
			record.push_back(action);

			poker->printCard();
			continue;
		}
		if (command == "dr")
		{
			ReleaseRecord();

			int suitNum;
			cout << "--deal randomly--" << endl << "input suitNum: "; cin >> suitNum;
			cout << endl;
			Action *action = new Deal(suitNum);
			action->Do(poker);

			((Deal*)action)->set(false);

			record.push_back(action);
			poker->printCard();
			continue;
		}
		if (command == "h")
		{
			showHelpInfo();
			continue;
		}
		if (command == "m")
		{
			testMove(poker);
			//printCard();
			poker->printCard();
			continue;
		}
		if (command == "p")
		{
			poker->printCard();
			//printCard();
			continue;
		}
		if (command == "r")
		{
			Action *action = new ReleaseCorner;
			action->Do(poker);

			record.push_back(action);

			poker->printCard();
			continue;
		}
		if (command == "redo")
		{
			if (record.size() > 1)
			{
				delete record.back();
				record.pop_back();
				Act();
			}
			else
				cout << "Can't redo." << endl;
			continue;
		}
		if (command == "test")
		{
			vector<string> vs;
			for (auto &r : record)
				vs.push_back(r->GetCommand());

			continue;
		}

		cout << "Unknowned Command" << endl;
		//cout << ">>";
	}
}