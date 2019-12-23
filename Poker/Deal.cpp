#include "Deal.h"
#include "Poker.h"

#include <algorithm>
#include <random>
#include <Windows.h>

#include <thread>
#include "Card.h"
#include "TImage.h"
#include "SequentialAnimation.h"
#include "SettingAnimation.h"
#include "ValueAnimation.h"

using namespace std;

//返回1维数组，各花色依次1-13点，共8*13=104张
vector<Card> Deal::genInitCard() const
{
	vector<Card> result;
	switch (suitNum)
	{
	case 1:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ 4, j });//1个花色：黑桃
		break;
	case 2:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ (i>3) ? 3 : 4, j });//2个花色：红桃，黑桃
		break;
	case 4:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ i % 4 + 1, j });//4个花色
		break;
	default:
		throw string("Error:'genInitCard(" + to_string(suitNum) + ")");
	}
	return result;
}

bool Deal::Do(Poker* inpoker)
{
	poker = inpoker;

	poker->suitNum = suitNum;
	poker->seed = seed;

	//先清理
	poker->desk.clear();
	poker->corner.clear();
	poker->finished.clear();

	std::default_random_engine e;

	//生成整齐牌
	auto cards = genInitCard();

	//打乱
	e.seed(seed);
	random_shuffle(cards.begin(), cards.end(), [&](int i){return e() % i; });

	//发牌
	int pos = 0;

	//4摞6张的=24
	for (int i = 0; i < 4; ++i)
	{
		vector<Card> deskOne;
		for (int j = 0; j < 6; ++j)
			deskOne.push_back(cards[pos++]);
		poker->desk.push_back(deskOne);
	}

	//6摞5张的=30
	for (int i = 0; i < 6; ++i)
	{
		vector<Card> deskOne;
		for (int j = 0; j < 5; ++j)
			deskOne.push_back(cards[pos++]);
		poker->desk.push_back(deskOne);
	}

	//5摞 待发区=50
	for (int i = 0; i < 5; ++i)
	{
		vector<Card> cornerOne;
		for (int j = 0; j < 10; ++j)
			cornerOne.push_back(cards[pos++]);
		poker->corner.push_back(cornerOne);
	}

	//每摞最外的牌亮牌
	for (auto &deskOne : poker->desk)
		deskOne.back().show = true;

	poker->score = 500;
	poker->operation = 0;

	return true;
}

void Deal::StartAnimation(HWND hWnd,bool &bOnAnimation,bool &bStopAnimation)
{
	//刷新牌的最后位置
	SendMessage(hWnd, WM_SIZE, 0, 0);

	SequentialAnimation* seq = new SequentialAnimation;
	POINT ptStart = poker->corner.back().back().GetPos();

	vector<AbstractAnimation*> vecFinal;
	for (int i = 0; i < 54; ++i)
	{
		int deskIndex = i % 10;
		int cardIndex = i / 10;

		auto& card = poker->desk[deskIndex][cardIndex];

		//所有牌设置为不可见
		card.SetVisible(false);

		//动画：设置z-index
		seq->Add(new SettingAnimation<Card, int>(&card,0,&Card::SetZIndex,999-i));

		//动画：设置为可见
		seq->Add(new SettingAnimation<Card, bool>(&card,0,&Card::SetVisible,true));

		//动画：从角落到指定位置
		seq->Add(new ValueAnimation<Card,POINT>(&card,25,&Card::SetPos,ptStart,card.GetPos()));

		card.SetPos(ptStart);

		//动画：恢复z-index
		seq->Add(new SettingAnimation<Card, int>(&card,0,&Card::SetZIndex,0));

		//所有牌设置为背面
		card.show = false;

		//最后10张牌
		if (cardIndex == poker->desk[deskIndex].size() - 1)
		{
			//背面翻到不显示
			vecFinal.push_back(new ValueAnimation<TImage, double>(&card.GetBackImage(),25,&TImage::SetIWidth,1.0,0.0));

			//动画：显示牌正面
			vecFinal.push_back(new SettingAnimation<Card, bool>(&card,0,&Card::SetShow,true));

			//正面翻出来
			vecFinal.push_back(new ValueAnimation<TImage, double>(&card.GetImage(),25,&TImage::SetIWidth,0.0,1.0));
		}
	}

	for (auto& ani : vecFinal)
		seq->Add(ani);

	//
	int msAll = 25 * 54+50*10 ;
	int times = msAll / 125+1;
	auto play = []()
	{
		PlaySound((LPCSTR)115, GetModuleHandle(NULL), SND_RESOURCE | SND_SYNC);
	};
	while (times--)
	{
		thread t(play);
		t.detach();
	}

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

bool Deal::Redo(Poker* inpoker)
{
	poker = inpoker;

	poker->desk.clear();
	poker->corner.clear();
	poker->finished.clear();

	return true;
}