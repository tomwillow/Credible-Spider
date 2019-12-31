#include "Manager.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <memory>
#include <unordered_set>
#include <typeinfo>

#include "resource.h"

#include "Add13.h"
#include "Deal.h"
#include "PMove.h"
#include "ReleaseCorner.h"
#include "Restore.h"


#ifndef _CONSOLE
#include "TImage.h"

#include "Firework.h"
#include "SequentialAnimation.h"
#include "ParallelAnimation.h"
#include "ValueAnimation.h"
#include "SettingAnimation.h"
#include "ValueAnimation.h"

#include "Configuration.h"
extern Configuration config;

#endif

#include <thread>
//#define _PRINT
//#define _PAUSE

using namespace std;

Manager::Manager() :poker(nullptr), bOnThread(false), bStopThread(false)
#ifndef _CONSOLE
, hasLoadImage(false), hWnd(NULL),
idCardEmpty(0), idCardBack(0), idCard1(0), idCardMask(0),
soundNoTip(0), soundTip(0), soundWin(0),
dragInfo()
#endif
{
}

Manager::Manager(int suitNum) : Manager()
{
	//poker = new Poker;

	//static default_random_engine e;
	//e.seed(e()+GetTickCount());
	//uint32_t seed = e();

	//Deal deal(suitNum, seed);
	//deal.Do(poker);

	//cout << *poker;
	Command("newrandom " + to_string(suitNum));
}

Manager::Manager(int suitNum, uint32_t seed) :Manager()
{
	//poker = new Poker;

	//Deal deal(suitNum, seed);
	//deal.Do(poker);

	//cout << *poker;

	Command("new " + to_string(suitNum) + " " + to_string(seed));
}

Manager::~Manager()
{
	delete poker;
	ReleaseRecord();

#ifndef _CONSOLE
	for (auto img : vecImgCardEmpty)
		delete img;
#endif
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
	shared_ptr<PMove> action(new PMove(orig, dest, num));
	bool success;
	if (success = action->Do(poker))
	{
		record.push_back(action);
	}

#ifndef _CONSOLE
	if (success && poker->hasGUI)
	{
		if (config.enableAnimation)
			action->StartAnimationQuick(hWnd, bOnThread, bStopThread);
	}
#else
	cout << (success ? "success." : "failed.") << endl;
	cout << *poker;
#endif
	return false;
}

void Manager::NewGame(istream& in, bool isRandom)
{

	int suitNum;
	uint32_t seed;
#ifdef _CONSOLE
	if (isRandom)
		cout << "--deal--" << endl << "input suitNum: ";
	else
		cout << "--deal--" << endl << "input suitNum, seed: ";
#endif

	if (isRandom)
	{
		in >> suitNum;

		static default_random_engine e;
		e.seed(e() + GetTickCount());
		seed = e();
	}
	else
	{
		in >> suitNum >> seed;
	}

	record.clear();
	delete poker;
	poker = new Poker;

	shared_ptr<Action> action(new Deal(suitNum, seed, config.enableSound, soundDeal));
	action->Do(poker);

#ifndef _CONSOLE
	if (idCardEmpty && idCardBack && idCard1 && idCardMask)
		InitialImage();
	if (poker->hasGUI)
	{
		SetWindowText(hWnd, (AppTitle + " 花色=" + to_string(suitNum) + " 种子=" + to_string(seed)).c_str());
		if (config.enableAnimation)
			action->StartAnimation(hWnd, bOnThread, bStopThread);
		else
		{
			RefreshPaint();
		}
	}
#else
	cout << *poker;
#endif
}

void Manager::NewGameSolved(istream& in)
{
	//RECT rect;
	//GetClientRect(hWnd, &rect);
	//InvalidateRect(hWnd, &rect, false);
	//UpdateWindow(hWnd);

	int suitNum;
	cout << "--deal--" << endl << "input suitNum: ";
	in >> suitNum;

	bool canBeSolved = false;

	uint32_t seed;

	default_random_engine e;
	e.seed(GetTickCount());
	while (1)
	{
		record.clear();
		delete poker;
		poker = new Poker;

		seed = e();
		shared_ptr<Action> action(new Deal(suitNum, seed, config.enableSound, soundDeal));
		action->Do(poker);

#ifndef _CONSOLE
		SetWindowText(hWnd, (AppTitle + " 花色=" + to_string(suitNum) + " seed=" + to_string(seed)).c_str());
#endif
		if (AutoSolve(false))
		{
			break;
		}
	}

#ifndef _CONSOLE
	stringstream ss;
	ss << "找到一组可解题目：" << endl << endl;
	ss << "花色=" << suitNum << endl;
	ss << "seed=" << seed << endl;
	ss << "尝试次数=" << autoSolveResult.calc << endl;
	ss << "难度=" << -10000.0 / autoSolveResult.calc + 100 << endl;

	MessageBox(hWnd, ss.str().c_str(), "求解结果", MB_OK | MB_ICONINFORMATION);
#endif
	Command("new " + to_string(suitNum) + " " + to_string(seed));

}

bool Manager::Command(const string command)
{
	istringstream iss(command);
	return ReadIn(iss);
}

#ifdef _CONSOLE
void Manager::ShowHelpInfo() const
{
	if (!poker)
	{
		cout << "--Command Line Help--" << endl;
		cout << "new : new game" << endl;
		cout << "newrandom : new random game" << endl;
		cout << "newsolved : new solved game" << endl;
		cout << "--Help End--" << endl << endl;
	}
	else
	{
		cout << "--Command Line Help--" << endl;
		cout << "auto" << endl;
		cout << "a add13" << endl;
		cout << "h help" << endl;
		cout << "m move" << endl;
		cout << "new : new game" << endl;
		cout << "newrandom : new random game" << endl;
		cout << "newsolved : new solved game" << endl;
		cout << "p print" << endl;
		cout << "r releaseCorner" << endl;
		cout << "redo" << endl;
		cout << "save" << endl;
		cout << "exit" << endl;
		cout << "--Help End--" << endl << endl;
	}
}
#endif

bool Manager::ReadIn(istream& in)
{
#ifdef _CONSOLE
	ShowHelpInfo();
#endif
	string command;
	bool success = true;
	while (1)
	{
		cout << ">>";
		if (!(in >> command))
			break;

		if (command == "new")
		{
			NewGame(in, false);
			continue;
		}
		if (command == "newrandom")
		{
			NewGame(in, true);
			continue;
		}
		if (command == "newsolved")
		{
			NewGameSolved(in);
			cout << *poker;
			continue;
		}

		if (poker == nullptr)
			continue;

		if (command == "auto")
		{
			int bPlayAnimation = 0;
			in >> bPlayAnimation;

			success = AutoSolve(bPlayAnimation);

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
#ifdef _CONSOLE
		if (command == "h")
		{
			showHelpInfo();
			continue;
		}
#endif
		if (command == "m")
		{
			success = Move(poker, in);
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
		if (command == "r")//release
		{
			shared_ptr<Action> action(new ReleaseCorner(config.enableSound, soundDeal));
			if (success = action->Do(poker))
			{
				record.push_back(action);
#ifndef _CONSOLE
				if (poker->hasGUI && config.enableAnimation)
				{
					action->StartAnimation(hWnd, bOnThread, bStopThread);
				}
#else
				cout << *poker;
#endif
			}
			continue;
		}
		if (command == "redo")
		{
			if (record.size() > 0)
			{
				record.back()->Redo(poker);
				//自动翻牌的撤销不加操作数，人工的要加
				poker->score -= 2;
				poker->operation += 2;

				record.pop_back();
				success = true;

#ifndef _CONSOLE
				if (poker->hasGUI)
				{
					//record.back()->RedoAnimation(hWnd, bOnThread, bStopThread);
					RECT rc;
					GetClientRect(hWnd, &rc);
					OnSize(rc);
					InvalidateRect(hWnd, &rc, false);
					UpdateWindow(hWnd);
				}
#else
				cout << *poker;
#endif
			}
#ifdef _CONSOLE
			else
				cout << "Can't redo." << endl;
#endif
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

std::vector<Manager::Node> Manager::GetAllOperator(std::vector<int>& emptyIndex, std::shared_ptr<Poker> poker, const unordered_set<Poker>& states)
{
	std::vector<Manager::Node> actions;
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
			//最面上的牌
			Card* pCardDest = &destCards.back();

			//逐个牌堆遍历
			for (int orig = 0; orig < poker->desk.size(); ++orig)
			{
				auto& origCards = poker->desk[orig];
				if (origCards.empty())
					continue;
				if (origCards.back().point >= pCardDest->point)
					continue;

				int num = 0;
				//从最底下的牌遍历到最顶上
				for (auto it = origCards.rbegin(); it != origCards.rend(); ++it)
				{
					num++;
					//点数不符合，不能移动
					if (it->point >= pCardDest->point)
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

					//it ----> pCard，目标pCard比源it大1
					//不考虑花色，花色留给估值函数计算
					if (it->point + 1 == pCardDest->point)//it->suit == pCard->suit && 
					{
						shared_ptr<Poker> tempPoker(new Poker(*poker));
						shared_ptr<Action> action(new PMove(orig, dest, num));
						if (action->Do(tempPoker.get()) &&
							(states.find(*tempPoker) == states.end()))
							actions.push_back({ tempPoker->GetValue(),tempPoker,action });
						break;
					}

				}
			}
		}
	}

	//没有空位，且待发区还有牌
	//加入发牌操作
	if (emptyIndex.empty() && !poker->corner.empty())
	{
		shared_ptr<Poker> newPoker(new Poker(*poker));
		shared_ptr<Action> action(new ReleaseCorner(config.enableSound, soundDeal));
		action->Do(newPoker.get());
		actions.push_back({ poker->GetValue() - 100,newPoker,action });
	}
	return actions;
}

bool Manager::DFS(bool& success, int& calc, const string& origTitle, vector<shared_ptr<Action>>& record, unordered_set<Poker>& states, int stackLimited, int calcLimited, bool playAnimation)
{
	if (poker->isFinished())
	{
		success = true;
		return true;
	}

	//操作次数超出限制，计算量超出限制
	if (poker->operation >= stackLimited || calc >= calcLimited || bStopThread)
	{
		return true;
	}

	calc++;

	auto ReleaseActions = [](vector<Node>& actions)
	{
		actions.clear();
	};



	vector<int> emptyIndex;
	shared_ptr<Poker> tempPoker(new Poker(*poker));
	vector<Node> actions = GetAllOperator(emptyIndex, tempPoker, states);

	//优化操作
	if (emptyIndex.empty())
		//没有空位
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
					if (cards.size() > 1 && cards.back().point < minPoint)
					{
						minPoint = cards.back().point;
						orig = i;
					}
				}

				//只添加一个移牌补空位的操作
				shared_ptr<Action> action(new PMove(orig, emptyIndex.front(), 1));
				shared_ptr<Poker> newPoker(new Poker(*poker));
				action->Do(newPoker.get());
				actions.push_back({ newPoker->GetValue(),newPoker,action });
			}
		}
	}

	//按照评估分大到小排序
	sort(actions.begin(), actions.end(), [](const Node& n1, const Node& n2) {return n1.value > n2.value; });

	static int round = -1;

	//开始递归
	for (auto it = actions.begin(); it != actions.end();)
	{
		auto& node = *it;

#ifdef _PRINT
		cout << *poker;

		//可以的操作
		cout << "Action:" << endl;
		for (auto it2 = it; it2 != actions.end(); ++it2)
			cout << *it2->action << " value:" << it2->value << endl;

		//显示操作
		cout << string(20, '-');
		cout << "Do:" << *node.action << endl;
#endif

#ifdef _PAUSE
		//此处暂停
		//按任意键则走一步，输入数字则直到stack==round才停下
		if (poker->operation < round)
		{
			;
		}
		else
		{
			string s;
			getline(cin, s);
			if (s.empty())
				round = -1;
			else
				round = stoi(s);
		}
#endif

		//没出现过的状态
		if (states.find(*node.poker) == states.end())
		{
			static bool bNounce = true;
			static bool bStop = false;
			node.action->Do(poker);


#ifndef _CONSOLE
			SetWindowText(hWnd, (origTitle + " (求解步骤=" + to_string(calc) + ")").c_str());
			if (poker->hasGUI)
			{

				if (playAnimation)
					node.action->StartAnimation(hWnd, bNounce, bStop);
				else
				{
					RECT rc;
					GetClientRect(hWnd, &rc);
					OnSize(rc);
					InvalidateRect(hWnd, &rc, false);
					UpdateWindow(hWnd);
					Sleep(1);
				}
			}
#endif
			//加入状态
			states.insert(*node.poker);

			//push记录
			record.push_back(node.action);

			if (DFS(success, calc, origTitle, record, states, stackLimited, calcLimited, playAnimation))
			{
				//只有终止才会返回true，如果任意位置返回true，此处将逐级终止递归
				ReleaseActions(actions);
				return true;
			}

			node.action->Redo(poker);

#ifndef _CONSOLE
			if (poker->hasGUI)
			{
				if (playAnimation)
					node.action->RedoAnimation(hWnd, bNounce, bStop);
				else
				{
					RECT rc;
					GetClientRect(hWnd, &rc);
					OnSize(rc);
					InvalidateRect(hWnd, &rc, false);
					UpdateWindow(hWnd);
					Sleep(1);
				}
			}
#endif

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

bool Manager::AutoSolve(bool playAnimation)
{
	bOnThread = true;
	bStopThread = false;

	unordered_set<Poker> states;
	autoSolveResult.calc = 0;
	autoSolveResult.success = false;
	autoSolveResult.suit = poker->suitNum;
	autoSolveResult.seed = poker->seed;

	//1花色时，200可以解出70/100个；500可以解出89/100个；1000可以解出92/100个；8000可以解出98/100个
	//2花色时，2000可以解出23/100个；8000可以解出32/100个
	//4花色时，100000解出0/6个
	int calcLimited = 2000;

	//480时栈溢出，所以必须小于480。不建议提高保留栈大小
	int stackLimited = 400;

#ifndef _CONSOLE
	char sz[MAX_PATH];
	GetWindowText(hWnd, sz, MAX_PATH);
	string origTitle(sz);
#else
	string origTitle = "";
#endif

	DFS(autoSolveResult.success, autoSolveResult.calc, origTitle, record, states, stackLimited, calcLimited, playAnimation);

#ifndef _CONSOLE
	SetWindowText(hWnd, origTitle.c_str());
#else
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
#endif
	bOnThread = false;
	return autoSolveResult.success;
}

bool Manager::CanRedo()
{
	return !record.empty();
}


#ifndef _CONSOLE
void Manager::SetSoundId(int idTip, int idNoTip, int idWin, int idDeal)
{
	soundTip = idTip;
	soundNoTip = idNoTip;
	soundWin = idWin;
	soundDeal = idDeal;
}

void Manager::SetTextOutputHWND(HWND hWnd)
{
	this->hWnd = hWnd;
}

void Manager::SetGUIProperty(HWND hWnd, int idCardEmpty, int idCardBack, int idCard1, int idCardMask)
{
	this->hWnd = hWnd;
	this->idCardEmpty = idCardEmpty;
	this->idCardBack = idCardBack;
	this->idCard1 = idCard1;
	this->idCardMask = idCardMask;

	//释放空牌位
	for (auto& img : vecImgCardEmpty)
		delete img;

	//创建空牌位
	vecImgCardEmpty.resize(10);
	for (auto& img : vecImgCardEmpty)
	{
		delete img;
		img = new TImage(GetModuleHandle(NULL), idCardEmpty);
	}
	hasLoadImage = true;
}

void Manager::InitialImage()
{
	assert(idCardEmpty && idCardBack && idCard1 && idCardMask);

	//每张牌加入图片
	for (auto& vec : poker->desk)
		for (auto& card : vec)
		{
			int imageIndex = (card.suit - 1) * 13 + card.point - 1;
			shared_ptr<TImage> imgCard(new TImage(GetModuleHandle(NULL), idCard1 + imageIndex, idCardMask));
			shared_ptr<TImage> imgCardBack(new TImage(GetModuleHandle(NULL), idCardBack, idCardMask));
			card.SetImage(imgCard, imgCardBack);
		}

	//角落牌加入图片
	for (auto& cards : poker->corner)
		for (auto& card : cards)
		{
			int imageIndex = (card.suit - 1) * 13 + card.point - 1;
			shared_ptr<TImage> imgCard(new TImage(GetModuleHandle(NULL), idCard1 + imageIndex, idCardMask));
			shared_ptr<TImage> imgCardBack(new TImage(GetModuleHandle(NULL), idCardBack, idCardMask));
			card.SetImage(imgCard, imgCardBack);
		}

	poker->hasGUI = true;
}

//计算空牌位坐标
POINT Manager::GetCardEmptyPoint(RECT rect, int index)
{
	int cardGap = (rect.right - cardWidth * 10) / 11;

	//空牌位位置
	int x = cardGap + index * (cardWidth + cardGap);
	int y = border;

	return { x,y };
}

bool Manager::PtInRelease(POINT pt)
{
	bool ret = false;
	for (auto& cards : poker->corner)
	{
		auto& card = cards.front();
		int x = card.GetPos().x;
		int y = card.GetPos().y;
		RECT rect{ x,y,x + cardWidth,y + cardHeight };
		if (PtInRect(&rect, pt))
			return true;
	}
	return ret;
}

void Manager::OnSize(RECT rect)
{
	if (hasLoadImage == false)
		return;

	int cardGap = (rect.right - cardWidth * 10) / 11;

	if (poker == nullptr)
	{
		//刷新空牌位位置
		for (int i = 0; i < vecImgCardEmpty.size(); ++i)
		{
			int x = cardGap + i * (cardWidth + cardGap);
			int y = border;

			vecImgCardEmpty[i]->pt.x = x;
			vecImgCardEmpty[i]->pt.y = y;
		}
	}
	else
	{
		//刷新桌牌位置
		for (int i = 0; i < poker->desk.size(); ++i)
		{
			int x = cardGap + i * (cardWidth + cardGap);
			int y = border;

			auto& cards = poker->desk[i];
			int cardY = y;
			for (auto it = cards.begin(); it != cards.end(); ++it)
			{
				it->SetPos({ x,cardY });
				if (it->show)//正面牌的y间距比背面牌大
					cardY += cardGapH * 2;
				else
					cardY += cardGapH;
			}
		}

		//刷新堆牌
		for (int i = 0; i < poker->corner.size(); ++i)
		{
			int cornerX = rect.right - cardGap - cardWidth - i * border;
			int cornerY = rect.bottom - border - cardHeight;
			for (auto& card : poker->corner[i])
			{
				card.SetPos({ cornerX,cornerY });

			}
		}

		//刷新完成牌
		for (int i = 0; i < poker->finished.size(); ++i)
		{
			int x = cardGap + i * xBorder;
			int y = rect.bottom - border - cardHeight;
			for (auto& card : poker->finished[i])
				card.SetPos({ x,y });
		}
	}

}

void Manager::Draw(HDC hdc, const RECT& rect)
{

	if (poker && poker->hasGUI)
	{
		vector<Card*> vecTopCards;

		//画完成牌
		for (auto& cards : poker->finished)
		{
			for (auto& card : cards)
				if (card.GetZIndex() > 0)
					vecTopCards.push_back(&card);
				else
					card.Draw(hdc);
		}

		//画堆牌
		for (auto& cards : poker->corner)
			for (auto& card : cards)
			{
				if (card.GetZIndex() > 0)
					vecTopCards.push_back(&card);
				else
					card.Draw(hdc);
			}

		//画桌牌
		for (auto& vec : poker->desk)
		{
			for (auto& card : vec)
			{
				if (card.GetZIndex() > 0)
					vecTopCards.push_back(&card);
				else
					card.Draw(hdc);
			}
		}


		//顶层牌按z-index从小到大排序
		sort(vecTopCards.begin(), vecTopCards.end(), [](const Card* c1, const Card* c2) {return c1->GetZIndex() < c2->GetZIndex(); });

		//画顶层牌
		for (auto& topCard : vecTopCards)
		{
			topCard->Draw(hdc);
		}

		//firework
		if (poker->isFinished())
		{
			int fireworkNum = poker->suitNum * 5;
			static vector<shared_ptr<Firework>> fireworks;

			//烟花变少则补充
			while (fireworks.size() < fireworkNum)
			{
				fireworks.push_back(make_shared<Firework>(rect.right, rect.bottom, config.enableSound, IDR_WAVE_FIREWORK));
			}

			for (auto& f : fireworks)
			{
				//烟花放完，用新烟花替换
				if (f->IsDead())
					f = make_shared<Firework>(rect.right, rect.bottom, config.enableSound, IDR_WAVE_FIREWORK);
				f->Draw(hdc);
			}
		}
	}
	else
	{
		//画空牌位
		for (auto& imgEmpty : vecImgCardEmpty)
			imgEmpty->Draw(hdc);
	}
}

bool Manager::ShowOneHint()
{
	static Poker prevPoker;
	static int i = 0;

	unordered_set<Poker> uSet;//空哈希表
	shared_ptr<Poker> tempPoker(make_shared<Poker>(*poker));//复制一个临时牌局供GetAllOperator释放
	vector<int> emptyIndex;

	//取得动作列表
	auto actions = GetAllOperator(emptyIndex, tempPoker, uSet);

	//无动作，或者仅有1个发牌动作
	if (actions.empty() ||
		(actions.size() == 1 && typeid(*actions.front().action) == typeid(ReleaseCorner)))
	{
		if (config.enableSound)
			PlaySound((LPCSTR)soundNoTip, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		return false;
	}

	if (config.enableSound)
		PlaySound((LPCSTR)soundTip, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

	//按照评估分大到小排序
	sort(actions.begin(), actions.end(), [](const Node& n1, const Node& n2) {return n1.value > n2.value; });

	if (!(prevPoker == *poker))//局面变化，重置i值
	{
		i = 0;
		prevPoker = *poker;
	}

	//如果当前动作是发牌，则向下推移、或者重置i
	if (typeid(*actions[i].action) == typeid(ReleaseCorner))
	{
		i++;
		if (i == actions.size())
			i = 0;
	}

	//因为仅有1个发牌的情况已经在前面过滤掉，所以必然会有PMove动作
	shared_ptr<PMove> pMove = dynamic_pointer_cast<PMove>(actions[i].action);

	//显示动画
	pMove->Do(poker);
	pMove->StartHintAnimation(hWnd, bOnThread, bStopThread);
	pMove->Redo(poker);

	RECT rect;
	GetClientRect(hWnd, &rect);
	InvalidateRect(hWnd, &rect, false);

	//递增i
	i++;
	if (i == actions.size())
		i = 0;


	return true;
}

void Manager::GetIndexFromPoint(int& deskIndex, int& cardIndex, POINT pt)
{
	deskIndex = -1, cardIndex = -1;
	if (poker == nullptr)
		return;

	for (int i = 0; i < poker->desk.size(); ++i)
	{
		for (int j = 0; j < poker->desk[i].size(); ++j)
		{
			auto& card = poker->desk[i][j];
			RECT rect = { card.GetPos().x,card.GetPos().y,card.GetPos().x + cardWidth,card.GetPos().y + cardHeight };
			if (PtInRect(&rect, pt))
			{
				deskIndex = i;
				cardIndex = j;
			}
		}
	}
}

bool Manager::OnLButtonDown(POINT pt)
{
	if (poker->hasGUI == false)
		return false;

	//取得按下的牌编号
	int deskIndex = -1, cardIndex = -1;
	GetIndexFromPoint(deskIndex, cardIndex, pt);

	//没有牌
	if (deskIndex == -1)
		return false;

	int num = poker->desk[deskIndex].size() - cardIndex;

	//不能够拾取
	if (!::CanPick(poker, deskIndex, num))
		return false;

	//开始拖动设置

	dragInfo.vecCard.clear();
	for (int i = 0; i < num; ++i)
	{
		//拖动组设置z-index，加入牌指针及相对坐标
		auto& card = poker->desk[deskIndex][cardIndex + i];
		card.SetZIndex(999);
		dragInfo.vecCard.push_back({ &card,card.GetPos() - pt });
	}

	dragInfo.bOnDrag = true;
	dragInfo.orig = deskIndex;
	dragInfo.num = num;
	dragInfo.cardIndex = cardIndex;

	return true;
}

void Manager::GiveUpDrag()
{
	dragInfo.bOnDrag = false;

	//恢复z-index
	for (auto& pr : dragInfo.vecCard)
	{
		pr.first->SetZIndex(0);
	}
	dragInfo.vecCard.clear();

	//恢复位置并刷新
	RECT rect;
	GetClientRect(hWnd, &rect);
	OnSize(rect);
	InvalidateRect(hWnd, &rect, false);

}

bool Manager::OnMouseMove(POINT pt)
{
	if (hasLoadImage == false)
		return false;

	if (dragInfo.bOnDrag)
	{
		//没有按下左键，释放拖动
		if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			GiveUpDrag();
		}
		else
		{
			//移动拖动组
			for (auto& pr : dragInfo.vecCard)
			{
				pr.first->SetPos(pt + pr.second);

			}

			//刷新
			RECT rect;
			GetClientRect(hWnd, &rect);
			InvalidateRect(hWnd, &rect, false);
			return true;
		}
	}
	return false;
}

bool Manager::OnLButtonUp(POINT pt)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	if (dragInfo.bOnDrag)
	{
		//取得拖动牌最顶上一张坐标
		POINT ptUpCard = dragInfo.vecCard.front().first->GetPos();
		auto GetDestIndex = [&](Poker* poker, RECT rectClient, POINT ptUpCard, int orig, int num)->int
		{
			int dest = -1;
			int Smax = -1;
			for (int i = 0; i < 10; ++i)
			{
				if (i == orig)//由于计算面积，自己和自己的面积必然最大，所以需要排除掉拖动源
					continue;

				POINT ptDest;//目标坐标
				if (poker->desk[i].empty())
					ptDest = GetCardEmptyPoint(rectClient, i);
				else
					ptDest = poker->desk[i].back().GetPos();

				int dx = abs(ptUpCard.x - ptDest.x);//坐标之差
				int dy = abs(ptUpCard.y - ptDest.y);
				int S = (cardWidth - dx) * (cardHeight - dy);//计算两个矩形的重合面积

				//第一个判断条件是为了排除掉负负得正的情形
				if (cardWidth - dx > 0 && S > Smax&& ::CanMove(poker, orig, i, num))
				{
					//在被覆盖牌中，取得可以放置的，且被覆盖面积最大的一张作为拖动目的地
					Smax = S;
					dest = i;
				}
			}
			return dest;
		};

		//取得目标牌位号
		int dest = GetDestIndex(poker, rect, ptUpCard, dragInfo.orig, dragInfo.num);

		//恢复拖动设置
		dragInfo.bOnDrag = false;
		for (auto& pr : dragInfo.vecCard)
		{
			pr.first->SetZIndex(0);
		}
		dragInfo.vecCard.clear();

		//有目标牌位，且可以移动
		if (dest != -1)
		{
			//进行移动
			Command("m " + to_string(dragInfo.orig) + " " + to_string(dest) + " " + to_string(dragInfo.num));

			OnSize(rect);
			InvalidateRect(hWnd, &rect, false);
			return true;
		}
	}
	OnSize(rect);
	InvalidateRect(hWnd, &rect, false);
	return false;
}

bool Manager::GetIsWon()
{
	return poker->isFinished();
}

void Manager::RefreshPaint()
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	OnSize(rect);
	InvalidateRect(hWnd, &rect, false);
}

void Manager::Win()
{
	if (config.enableSound)
		PlaySound((LPCSTR)soundWin, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

	record.clear();

	auto fun = [&]()
	{
		bOnThread = true;
		bStopThread = false;
		while (bStopThread == false)
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			InvalidateRect(hWnd, &rect, false);
			//UpdateWindow(hWnd);
			//PostMessage(hWnd, WM_PAINT, 0, 0);
			Sleep(25);//50 fps
		}
		bOnThread = false;
	};

	thread t(fun);
	t.detach();
}
#endif