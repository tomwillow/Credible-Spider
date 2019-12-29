#include "ReleaseCorner.h"

#include "Poker.h"
#include "Restore.h"
#include <assert.h>

#include <thread>
#include "resource.h"
#include "Card.h"

#ifndef _CONSOLE
#include "TImage.h"
#include "SequentialAnimation.h"
#include "ValueAnimation.h"
#include "SettingAnimation.h"
#include "ValueAnimation.h"
#include "CardTurnOverAnimation.h"
#endif

using namespace std;

ReleaseCorner::~ReleaseCorner()
{
}

//释放一摞右下角，检查收牌情况
bool ReleaseCorner::Do(Poker* inpoker)
{
	poker = inpoker;

	//角落区没牌
	if (poker->corner.empty())
		return false;

	//有空位不能发牌
	for (auto& cards : poker->desk)
		if (cards.empty())
			return false;

	//取得角落区坐标
	if (poker->hasGUI)
		ptStart = poker->corner.back().back().GetPos();

	//遍历一摞待发区牌
	for (int i = 0; i < 10; ++i)
	{
		//待发区亮牌
		poker->corner.back()[i].show = true;

		//逐个堆叠加上
		poker->desk[i].push_back(poker->corner.back()[i]);

	}

	//去掉一摞待发区
	poker->corner.pop_back();
	success = true;

	poker->score--;
	poker->operation++;

	//进行回收
	restored = make_shared<Restore>();
	if (restored->Do(poker) == false)
		restored = nullptr;

	return true;
}

bool ReleaseCorner::Redo(Poker* inpoker)
{
	assert(success);

	poker = inpoker;

	//撤销回收
	if (restored)
	{
		restored->Redo(poker);
	}

	poker->score++;
	poker->operation--;

	//回收10张牌
	vector<Card> temp;
	for (int i = 0; i < 10; ++i)
	{
		//改为背面
		poker->desk[i].back().show = false;

		//回收
		temp.push_back(poker->desk[i].back());

		//从桌上取掉
		poker->desk[i].pop_back();
	}

	poker->corner.push_back(temp);
	return true;
}


#ifndef _CONSOLE
void ReleaseCorner::StartAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation)
{
	assert(poker->hasGUI);
	assert(success);
	//如果发生了回收事件，先恢复到回收前
	if (restored)
		restored->Redo(poker);

	//刷新牌的最后位置
	SendMessage(hWnd, WM_SIZE, 0, 0);

	shared_ptr<SequentialAnimation> seq(make_shared<SequentialAnimation>());

	vector<AbstractAnimation*> vecFinalAni;
	for (int i = 0; i < 10; ++i)
	{
		auto& card = poker->desk[i].back();

		//所有牌设置为可见
		card.SetVisible(true);

		//第一张在最上面
		card.SetZIndex(999-i);

		//动画：设置为可见
		seq->Add(new SettingAnimation<Card, bool>(&card,0,&Card::SetVisible,true));

		//动画：从角落到指定位置
		seq->Add(new ValueAnimation<Card,POINT>(&card,25,&Card::SetPos,ptStart,card.GetPos()));

		vecStartPos.push_back(ptStart);
		vecEndPos.push_back(card.GetPos());

		card.SetPos(ptStart);

		//从背面翻出来
		auto temp=CardTurnOverAnimation::AddBackToFrontAnimation(card);
		vecFinalAni.insert(vecFinalAni.end(), temp.begin(), temp.end());

		//动画：恢复z-index
		seq->Add(new SettingAnimation<Card, int>(&card,0,&Card::SetZIndex,0));
	}

	seq->Add(vecFinalAni);

	//
	int msAll = 75*10;
	int times = msAll / 125 + 1;
	auto play = []()
	{
		PlaySound((LPCSTR)IDR_WAVE_DEAL, GetModuleHandle(NULL), SND_RESOURCE | SND_SYNC);
	};
	while (times--)
	{
		thread t(play);
		t.detach();
	}

		bStopAnimation = false;
		bOnAnimation = true;
		seq->Start(hWnd, bStopAnimation);

		//如果在Do中发生了回收，此时再进行回收
		if (restored)
		{
			restored->Do(poker);
			restored->StartAnimation(hWnd, bOnAnimation, bStopAnimation);
		}
		bOnAnimation = false;
}

void ReleaseCorner::RedoAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation)
{
	assert(poker->hasGUI);
	SequentialAnimation* seq = new SequentialAnimation;

	for (int i = 0; i < 10; ++i)
	{
		//最后一摞每张牌
		auto& card = poker->corner.back()[i];

		//所有牌设置为可见
		card.SetVisible(true);

		//所有牌设置为发完牌的位置
		card.SetPos(vecEndPos[i]);

		//动画时设置为顶层
		card.SetZIndex(999);

		//从正面翻回背面
		seq->Add(CardTurnOverAnimation::AddFrontToBackAnimation(card));

		//动画：从角落到指定位置
		seq->Add(new ValueAnimation<Card, POINT>(&card,25,&Card::SetPos,vecEndPos[i],vecStartPos[i]));

		//恢复z-index
		seq->Add(new SettingAnimation<Card, int>(&card, 0, &Card::SetZIndex, 0));
	}

	vecStartPos.clear();
	vecEndPos.clear();

	//
	int msAll = 75 * 10;
	int times = msAll / 125 + 1;
	auto play = []()
	{
		PlaySound((LPCSTR)IDR_WAVE_DEAL, GetModuleHandle(NULL), SND_RESOURCE | SND_SYNC);
	};
	while (times--)
	{
		thread t(play);
		t.detach();
	}

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
}
#endif