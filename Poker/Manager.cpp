#include "Manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <memory>
#include <unordered_set>

#include "Add13.h"
#include "Deal.h"
#include "PMove.h"
#include "ReleaseCorner.h"
#include "Restore.h"

//#define _PRINT

using namespace std;

Manager::Manager(int suitNum)
{
	poker = new Poker;

	Deal deal(suitNum);
	deal.Do(poker);

	cout << *poker;
}

Manager::Manager(int suitNum, int seed)
{
	poker = new Poker;

	Deal deal(suitNum, seed);
	deal.Do(poker);

	cout << *poker;
}

Manager::~Manager()
{
	delete poker;
	ReleaseRecord();
}

void Manager::ReleaseRecord()
{
	record.clear();
}

bool Manager::CanPick(Poker* poker, istream& in)
{
	int orig, num;
	cout << "--canPick--" << endl << "input orig, num: ";
	in >> orig >> num;
	cout << endl;
	cout << "canPick? ";
	if (::CanPick(poker, orig, num))
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

bool Manager::Move(Poker* poker, istream& in)
{
	int orig, dest, num;
	cout << "--move--" << endl << "input orig, dest, num: "; in >> orig >> dest >> num;
	cout << endl;
	cout << "Chose: "; poker->printCard(orig, num);
	cout << endl;
	cout << "canMove? ";
	shared_ptr<Action> action ( new PMove(orig, dest, num));
	if (action->Do(poker))
	{
		record.push_back(action);
		cout << "success." << endl;
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

bool Manager::readIn(istream& in)
{
	showHelpInfo();
	string command;
	bool success = true;
	while (1)
	{
		cout << ">>";
		if (!(in >> command))
			break;
		if (command == "auto")
		{
			AutoSolve();
			break;
			//continue;
		}
		if (command == "a")
		{
			int deskNum;
			cout << "--add13--" << endl << "input deskNum: "; in >> deskNum;
			cout << endl;
			shared_ptr<Action> action(new Add13(deskNum));
			if (action->Do(poker))
			{
				record.push_back(action);
				cout << *poker;
			}
			continue;
		}
		if (command == "h")
		{
			showHelpInfo();
			continue;
		}
		if (command == "m")
		{
			success = Move(poker, in);

			cout << *poker;
			continue;
		}
		if (command == "pick")
		{
			success = CanPick(poker, in);
			continue;
		}
		if (command == "p")
		{
			cout << *poker;
			continue;
		}
		if (command == "r")
		{
			shared_ptr<Action> action ( new ReleaseCorner());
			if (action->Do(poker))
			{
				record.push_back(action);
				cout << *poker;
			}
			continue;
		}
		if (command == "redo")
		{
			if (record.size() > 0)
			{
				record.back()->Redo(poker);
				record.pop_back();
				success = true;

				cout << *poker;
			}
			else
				cout << "Can't redo." << endl;
			continue;
		}
		if (command == "save")
		{
			ofstream ofs("1.txt");
			for (auto& action : record)
			{
				ofs << *action;
			}
			ofs.close();
			continue;
		}
		if (command == "test")
		{
			vector<string> vs;
			for (auto& r : record)
				vs.push_back(r->GetCommand());

			continue;
		}
		if (command == "exit")
		{
			break;
		}

		cout << "Unknowned Command" << endl;
		throw "Error:Unknowned Command";
		//cout << ">>";
	}
	return success;
}

void Manager::showHelpInfo() const
{
	cout << "--Command Line Help--" << endl;
	cout << "auto" << endl;
	cout << "a add13" << endl;
	cout << "h help" << endl;
	cout << "m move" << endl;
	cout << "p print" << endl;
	cout << "r releaseCorner" << endl;
	cout << "redo" << endl;
	cout << "save" << endl;
	cout << "exit" << endl;
	cout << "--Help End--" << endl << endl;
}

bool dfs(Poker &result,bool &success,int &calc,shared_ptr<Poker> poker,vector<shared_ptr<Action>> &record, unordered_set<Poker>& states,int stackLimited,int calcLimited)
{
	if (poker->isFinished())
	{
		result = *poker;
		success = true;
		return true;
	}

	if (poker->operation >= stackLimited || calc>=calcLimited)
	{
		return true;
	}

	calc++;

	struct Node
	{
		int value;
		shared_ptr<Poker> poker;
		shared_ptr<Action> action;
	};
	vector<Node> actions;

	auto ReleaseActions = [](vector<Node> &actions)
	{
		actions.clear();
	};

	vector<int> emptyIndex;
	for (int dest = 0; dest < poker->desk.size(); ++dest)
	{
		auto& destCards = poker->desk[dest];
		if (destCards.empty())
		{
			emptyIndex.push_back(dest);

			//当前牌堆为空
			//遍历所有牌堆，把非空的加进来
			for (int orig = 0; orig < poker->desk.size(); ++orig)
			{
				auto& cards = poker->desk[orig];
				if (cards.empty())
					continue;

				//得到可移动牌数量
				//至少会返回1
				int num = 1;
				while (1)
				{
					if (num > cards.size())
					{
						num--;
						break;
					}
					if (::CanPick(poker.get(), orig, num))
					{
						num++;
					}
					else
					{
						num--;
						break;
					}
				}

				//全部移到空位没有意义
				if (num == cards.size())
					continue;

				//
				shared_ptr<Poker> newPoker(new Poker(*poker.get()));
				shared_ptr<Action> action(new PMove(orig, dest, num));
				action->Do(newPoker.get());

				if (states.find(*newPoker) == states.end())
					actions.push_back({ newPoker->GetValue(),newPoker,action });
			}
		}
		else//dest牌堆非空
		{
			//最底下的牌
			Card* pCard = &destCards.back();

			//逐个牌堆遍历
			for (int orig = 0; orig < poker->desk.size(); ++orig)
			{
				auto& origCards = poker->desk[orig];
				if (origCards.empty())
					continue;
				if (&origCards.back() == pCard)
					continue;

				int num = 0;
				//从最底下的牌遍历到最顶上
				for (auto it = origCards.rbegin(); it != origCards.rend(); ++it)
				{
					num++;
					//点数不符合，不能移动
					if (it->point >= pCard->point)
						break;

					//没有显示的牌，不能移动
					if (it->show == false)
						break;

					//不是倒数第1个
					if (it != origCards.rbegin())
					{
						auto itDown = it - 1;//上一张牌
						if (itDown->point + 1 != it->point)//不连续则跳出，不能移动
							break;
					}

					//it ----> pCard，目标比源大1
					//不考虑花色，花色留给估值函数计算
					if (it->point + 1 == pCard->point)//it->suit == pCard->suit && 
					{
						shared_ptr<Poker> tempPoker(new Poker(*poker.get()));
						shared_ptr<Action> action( new PMove(orig, dest, num));
						action->Do(tempPoker.get());

						if (states.find(*tempPoker) == states.end())
							actions.push_back({ tempPoker->GetValue(),tempPoker,action });
						break;
					}

				}
			}
		}
	}

	//待发区还有牌
	//加入发牌操作
	if (!poker->corner.empty())
	{
		shared_ptr<Poker> newPoker(new Poker(*poker.get()));
		shared_ptr<Action> action(new ReleaseCorner());
		action->Do(newPoker.get());
		actions.push_back({ poker->GetValue()-100,newPoker,action });
	}

	//没有空位
	if (emptyIndex.empty())
		//去掉比当前评分还低的移牌
		for (auto it = actions.begin(); it != actions.end();)
		{
			if (typeid(*it->action) == typeid(PMove) && it->value <= poker->GetValue())
			{
				it = actions.erase(it);
			}
			else
				it++;
		}
	else
	{
		//有空位
		//如果待发区还有牌，则移牌到空位补空，因为有空位不能发牌
		if (!poker->corner.empty())
		{
			//如果全是顺牌，则找一张最小的移过去
			bool AllIsOrdered = true;
			for (auto& cards : poker->desk)
			{
				for (int i = 1; i < cards.size(); ++i)
				{
					if (cards[i - 1].suit != cards[i].suit || cards[i - 1].point - 1 != cards[i].point)
					{
						AllIsOrdered = false;
						break;
					}
				}
			}

			if (AllIsOrdered)
			{
				//清空当前所有操作
				ReleaseActions(actions);
				int orig = 0;
				int minPoint = 14;
				//寻找最小的牌
				for (int i = 0; i < poker->desk.size(); ++i)
				{
					auto& cards = poker->desk[i];
					if (!cards.empty() && cards.back().point < minPoint)
					{
						minPoint = cards.back().point;
						orig = i;
					}
				}

				//只添加一个移牌补空位的操作
				shared_ptr<Action> action(new PMove(orig, emptyIndex.front(), 1));
				shared_ptr<Poker> newPoker(new Poker(*poker.get()));
				action->Do(newPoker.get());
				actions.push_back({ newPoker->GetValue(),newPoker,action });
			}
		}
	}

	//按照评估分大到小排序
	sort(actions.begin(), actions.end(), [](const Node& n1, const Node& n2) {return n1.value > n2.value; });

	static int round = -1;

	//开始递归
	for (auto it=actions.begin();it!=actions.end();)
	{
		auto& node = *it;

#ifdef _PRINT
		cout << *poker;

		//可以的操作
		cout << "Action:" << endl;
		for (auto it2=it;it2!=actions.end();++it2)
			cout << *it2->action << " value:" << it2->value << endl;

		//显示操作
		cout << string(20, '-');
		cout << "Do:" << *node.action << endl;
#endif

		//此处暂停
		//按任意键则走一步，输入数字则直到stack==round才停下
		//if (poker->operation < round)
		//{
		//	;
		//}
		//else
		//{
		//	string s;
		//	getline(cin, s);
		//	if (s.empty())
		//		round = -1;
		//	else
		//		round = stoi(s);
		//}

		//没出现过的状态
		if (states.find(*node.poker) == states.end())
		{
			//加入状态
			states.insert(*node.poker);

			//push记录
			record.push_back(node.action);

			if (dfs(result,success,calc,node.poker,record, states,stackLimited,calcLimited))
			{
				//只有终止才会返回true，如果任意位置返回true，此处将逐级终止递归
				ReleaseActions(actions);
				return true;
			}

			//pop记录
			record.pop_back();

			it++;
		}
		else//已出现过的状态
		{
#ifdef _PRINT
			cout << string(20, '-');
			cout << "No-movement:" << endl;
#endif
			//直接转到下一个操作
			it = actions.erase(it);
		}
	}

	ReleaseActions(actions);
	return false;
}

bool Manager::AutoSolve()
{
	unordered_set<Poker> states;
	bool success = false;
	int calc = 0;

	//1花色时，200可以解出70/100个；500可以解出89/100个；1000可以解出92/100个；8000可以解出98/100个
	//2花色时，2000可以解出23/100个；8000可以解出32/100个
	//4花色时，100000解出0/6个
	int calcLimited = 200;

	//480时栈溢出，所以必须小于480。不建议提高保留栈大小
	int stackLimited = 400;

	//因为传入的poker是智能指针，会被释放
	//所以新建一个Poker保存完成状态
	//未完成的话Poker是空的，因为dfs只有true才写入result
	Poker* result = new Poker;
	dfs(*result,success,calc,shared_ptr<Poker>(poker),record, states,stackLimited,calcLimited);

	poker = result;

	//输出计算量
	cout << "Calculation:" << calc << endl;

	if (success == true)
	{
		//输出步骤
		cout << "Finished. Step = " << record.size() << endl;
		for (int i = 0; i < record.size(); ++i)
			cout << "[" << i << "] " << *record[i] << endl;
	}
	else
	{
		//输出失败原因
		if (calc >= calcLimited)
			cout << "Calculation number >= " << calcLimited << endl;
		else
			cout << "Call-stack depth >= " << stackLimited << endl;
		cout << "Fail." << endl;
	}
	return success;
}

bool Manager::CanRedo()
{
	return record.size() > 1;
}
