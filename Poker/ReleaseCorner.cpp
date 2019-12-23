#include "ReleaseCorner.h"

#include "Poker.h"
#include "Restore.h"
#include <assert.h>

#include <thread>
#include "Card.h"
#include "TImage.h"
#include "SequentialAnimation.h"
#include "ValueAnimation.h"
#include "SettingAnimation.h"
#include "ValueAnimation.h"
using namespace std;

ReleaseCorner::~ReleaseCorner()
{
	delete restored;
}

//释放一摞右下角，检查收牌情况
bool ReleaseCorner::Do(Poker* inpoker)
{
	poker = inpoker;

	if (poker->corner.empty())
		return false;

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

	//刷新看能否回收
	restored = new Restore();
	if (restored->Do(poker))
		;
	else
	{
		delete restored;
		restored = nullptr;
	}

	return true;
}

bool ReleaseCorner::Redo(Poker* inpoker)
{
	assert(success);

	poker = inpoker;

	//
	if (restored)
	{
		restored->Redo(poker);
		delete restored;
		restored = nullptr;
	}

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



void ReleaseCorner::StartAnimation(HWND hWnd, POINT ptStart, bool& bOnAnimation, bool& bStopAnimation)
{
	//刷新牌的最后位置
	SendMessage(hWnd, WM_SIZE, 0, 0);

	SequentialAnimation* seq = new SequentialAnimation;

	for (int i = 0; i < 10; ++i)
	{
		auto& card = poker->desk[i].back();


		////所有牌设置为背面
		card.show = false;

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


		//背面翻到不显示
		seq->Add(new ValueAnimation<TImage, double>(&card.GetBackImage(),25,&TImage::SetIWidth,1.0,0.0));

		//动画：显示牌正面
		seq->Add(new SettingAnimation<Card, bool>(&card,0,&Card::SetShow,true));

		//正面翻出来
		seq->Add(new ValueAnimation<TImage, double>(&card.GetImage(),25,&TImage::SetIWidth,0.0,1.0));

		//动画：设置为顶层
		seq->Add(new SettingAnimation<Card, int>(&card,0,&Card::SetZIndex,0));
	}

	//
	int msAll = 75*10;
	int times = msAll / 125 + 1;
	auto play = []()
	{
		PlaySound((LPCSTR)115, GetModuleHandle(NULL), SND_RESOURCE | SND_SYNC);
	};
	while (times--)
	{
		thread t(play);
		t.detach();
	}

	//seq->Start(hWnd);
	auto fun = [&](SequentialAnimation* seq, HWND hWnd)
	{
		bStopAnimation = false;
		bOnAnimation = true;
		seq->Start(hWnd, bStopAnimation);
		delete seq;
		bOnAnimation = false;
	};

	thread t(fun, seq, hWnd);
	t.detach();
}

void ReleaseCorner::RedoAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation)
{
	SequentialAnimation* seq = new SequentialAnimation;

	for (int i = 0; i < 10; ++i)
	{
		//最后一摞每张牌
		auto& card = poker->corner.back()[i];

		//所有牌设置为可见
		card.SetVisible(true);

		//所有牌设置为正面
		card.SetShow(true);

		//所有牌设置为发完牌的位置
		card.SetPos(vecEndPos[i]);

		//正面翻到不显示
		seq->Add(new ValueAnimation<TImage, double>(&card.GetImage(),25,&TImage::SetIWidth,1.0,0.0));

		//动画：显示牌背面
		seq->Add(new SettingAnimation<Card, bool>(&card,0,&Card::SetShow,false));

		//背面翻出来
		seq->Add(new ValueAnimation<TImage, double>(&card.GetBackImage(),25,&TImage::SetIWidth,0.0,1.0));

		//动画：从角落到指定位置
		seq->Add(new ValueAnimation<Card, POINT>(&card,25,&Card::SetPos,vecEndPos[i],vecStartPos[i]));

	}

	vecStartPos.clear();
	vecEndPos.clear();

	//
	int msAll = 75 * 10;
	int times = msAll / 125 + 1;
	auto play = []()
	{
		PlaySound((LPCSTR)115, GetModuleHandle(NULL), SND_RESOURCE | SND_SYNC);
	};
	while (times--)
	{
		thread t(play);
		t.detach();
	}

	//seq->Start(hWnd);
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

	thread t(fun, seq, hWnd);
	t.detach();
}