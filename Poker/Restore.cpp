#include "Restore.h"
#include "Poker.h"

#include <assert.h>
#include <vector>

#include <thread>
#include "Card.h"
#include "TImage.h"
#include "SequentialAnimation.h"
#include "ValueAnimation.h"
#include "SettingAnimation.h"
#include "ValueAnimation.h"
#include "CardTurnOverAnimation.h"
using namespace std;

//返回对应堆叠能否回收
bool Restore::CanRestore(Poker* poker,int deskNum) const
{
	if (poker->desk[deskNum].empty())
		return false;
	int pos = poker->desk[deskNum].size() - 1;
	int suit = poker->desk[deskNum][pos].suit;

	//i是点数
	for (int i = 1; i <= 13; ++i)
	{
		//从最后一张牌开始，点数升序，花色一致 则可以回收
		if (pos >= 0 && poker->desk[deskNum][pos].point == i && poker->desk[deskNum][pos].suit == suit)
		{
			pos--;
			continue;
		}
		else
			return false;
	}
	return true;
}

bool Restore::DoRestore(Poker* poker,int deskNum)
{
	if (CanRestore(poker,deskNum))
	{
		Oper oper;
		oper.origDeskIndex = deskNum;
		//进行回收

		//加入套牌，从最低下一张倒数13张，所以顺序为1-13
		vector<Card> temp(poker->desk[deskNum].rbegin(), poker->desk[deskNum].rbegin() + 13);
		poker->finished.push_back(temp);

		//预存起点位置
		if (temp.front().HasImage())
			for_each(temp.begin(), temp.end(), [&oper](const Card& card) {oper.vecStartPt.push_back(card.GetPos()); });

		//去掉牌堆叠的13张
		poker->desk[deskNum].erase(poker->desk[deskNum].end() - 13, poker->desk[deskNum].end());

		//翻开下面的牌
		if (!poker->desk[deskNum].empty() && poker->desk[deskNum].back().show == false)
		{
			poker->desk[deskNum].back().show = true;
			oper.shownLastCard = true;
		}
		else
			oper.shownLastCard = false;

		poker->score += 100;

		vecOper.push_back(oper);

		return true;
	}
	return false;
}


bool Restore::Do(Poker* inpoker)
{
	poker = inpoker;

	if (vecOper.empty())
		//未指定回收组号
		//扫描每个堆叠寻找能回收的组
		for (size_t i = 0; i < poker->desk.size(); ++i)
		{
			DoRestore(poker,i);
		}
	else
	{
		//已指定回收组号
		int deskIndex = vecOper.front().origDeskIndex;
		vecOper.clear();
		DoRestore(poker,deskIndex);
	}

	return !vecOper.empty();
}

void Restore::StartAnimation(bool blocking, HWND hWnd, bool& bOnAnimation, bool& bStopAnimation)
{
	//刷新终点位置
	SendMessage(hWnd, WM_SIZE, 0, 0);

	SequentialAnimation* seq = new SequentialAnimation;

	//每个回收组
	for (auto& oper : vecOper)
	{
		//终点
		oper.ptEnd = poker->finished.back().back().GetPos();

		//1-13
		for (int i=0;i<poker->finished.back().size();++i)
		{
			auto& card = poker->finished.back()[i];

			//设置起点
			card.SetPos(oper.vecStartPt[i]);

			//由于finished图层顺序为13-1，此处z-index反向，图层顺序改为1-13
			card.SetZIndex(13 - i);

			//移动
			seq->Add(new ValueAnimation<Card, POINT>(&card, 25, &Card::SetPos, oper.vecStartPt[i], oper.ptEnd));

			//恢复z-index
			seq->Add(new SettingAnimation<Card, int>(&card, 0, &Card::SetZIndex, 0));
		}

		if (oper.shownLastCard)
		{
			auto& card = poker->desk[oper.origDeskIndex].back();
			seq->Add(CardTurnOverAnimation::AddBackToFrontAnimation(card));
		}
	}


	auto fun = [&](SequentialAnimation* seq, HWND hWnd)
	{
		bStopAnimation = false;
		bOnAnimation = true;
		seq->Start(hWnd, bStopAnimation);
		delete seq;
		bOnAnimation = false;
		SendMessage(hWnd, WM_SIZE, 0, 0);
		RECT rc;
		GetClientRect(hWnd, &rc);
		InvalidateRect(hWnd, &rc, false);
		UpdateWindow(hWnd);
	};

	if (blocking)
	{
		fun(seq, hWnd);
	}
	else
	{
		thread t(fun, seq, hWnd);
		t.detach();
	}
}

bool Restore::Redo(Poker* inpoker)
{
	assert(vecOper.size());

	poker = inpoker;

	for (auto it = vecOper.rbegin(); it != vecOper.rend(); ++it)
	{
		poker->score -= 100;

		//如果翻过牌则翻回去
		if (it->shownLastCard)
			poker->desk[it->origDeskIndex].back().show = false;

		//把完成的牌放回堆叠
		auto it1 = poker->finished.back().rbegin();
		auto it2 = poker->finished.back().rend();
		poker->desk[it->origDeskIndex].insert(poker->desk[it->origDeskIndex].end(), it1, it2);

		//完成的牌消掉
		poker->finished.pop_back();
	}

	vecOper.clear();
	return true;
}