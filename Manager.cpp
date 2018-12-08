#include "Manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

#include "Poker\Add13.h"
#include "Poker\Deal.h"
#include "Poker\PMove.h"
#include "Poker\ReleaseCorner.h"

using namespace std;

Manager::Manager()
{
	poker = NULL;
	pCmdFunc = NULL;
}


Manager::~Manager()
{
	if (poker!=NULL)
		delete poker;
	ReleaseRecord();
}

void Manager::ReleaseRecord()
{
	for (auto &r : record)
		delete r;
	record.clear();
}

bool Manager::canPick(Poker *poker, istream &in)
{
	int orig, start;
	cout << "--canPick--" << endl << "input orig, start, dest: "; in >> orig >> start;
	cout << endl;
	cout << "Chose: "; poker->printCard(orig, start); cout << endl;
	cout << "canPick? ";
	Action *action = new PMove(orig, start);
	if (action->Do(poker))
	{
		cout << "Can." << endl;
		return true;
	}
	else
	{
		cout << "Can't." << endl;
		return false;
	}
}
bool Manager::move(Poker *poker, istream &in)
{
	int orig, start, dest;
	cout << "--move--" << endl << "input orig, start, dest: "; in >> orig >> start >> dest;
	cout << endl;
	cout << "Chose: "; poker->printCard(orig, start); cout << endl;
	cout << "canMove? ";
	Action *action = new PMove(orig, start, dest);
	if (action->Do(poker))
	{
		cout << "success." << endl;
		record.push_back(action);
		return true;
	}
	else
	{
		cout << "failed." << endl;
		return false;
	}
}

bool Manager::Command(const string command)
{
	istringstream iss(command);
	return readIn(iss);
}

bool Manager::readIn(istream &in)
{
	showHelpInfo();
	string command;
	//cout << ">>";
	bool success=true;
	while (in >> command)
	{
		cout << ">>";
		if (command == "act")
		{
			Act();
			continue;
		}
		if (command == "a")
		{
			int deskNum;
			cout << "--add13--" << endl << "input deskNum: "; in >> deskNum;
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

			if (poker) delete poker;
			poker = new Poker;

			int suitNum, seed;
			cout << "--deal with seed--" << endl << "input suitNum, seed: "; in >> suitNum >> seed;
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
			
			if (poker) delete poker;
			poker = new Poker;

			int suitNum;
			cout << "--deal randomly--" << endl << "input suitNum: "; in >> suitNum;
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
			success=move(poker,in);

			poker->printCard();
			continue;
		}
		if (command == "pick")
		{
			success = canPick(poker, in);
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
				success = true;
			}
			else
				cout << "Can't redo." << endl;
			continue;
		}
		if (command == "s")
		{
			ofstream ofs("1.txt");
			for (auto &action : record)
			{
				action->Output(ofs);
			}
			ofs.close();
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
		throw "Error:Unknowned Command";
		//cout << ">>";
	}
	if (pCmdFunc!=NULL)
		pCmdFunc();
	return success;
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
	cout << "s save" << endl;
	cout << "--Help End--" << endl << endl;
}

void Manager::autoSolve()
{
	if (poker->isFinished())
		return;

	vector<tuple<int, int, int>> moveList;

}

bool Manager::canRedo()
{
	return record.size() > 1;
}



Action* Manager::GetLastAct()
{
	if (record.empty())
		return NULL;
	else
		return record.back();
}