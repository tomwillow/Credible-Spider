#include "Deal.h"
#include "Poker.h"

#include <algorithm>
#include <random>
#include <Windows.h>

#include <thread>
#include "resource.h"
#include "Card.h"
#include "TImage.h"

#ifndef _CONSOLE
#include "SequentialAnimation.h"
#include "SettingAnimation.h"
#include "ValueAnimation.h"
#endif

using namespace std;

Deal::Deal(int suitNum, uint32_t seed) :Action(), suitNum(suitNum), seed(seed)
{

}

//����1ά���飬����ɫ����1-13�㣬��8*13=104��
vector<Card> Deal::genInitCard() const
{
	vector<Card> result;
	switch (suitNum)
	{
	case 1:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ 4, j });//1����ɫ������
		break;
	case 2:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ (i>3) ? 3 : 4, j });//2����ɫ�����ң�����
		break;
	case 4:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ i % 4 + 1, j });//4����ɫ
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

	//������
	poker->desk.clear();
	poker->corner.clear();
	poker->finished.clear();

	std::default_random_engine e;

	//����������
	auto cards = genInitCard();

	//����
	e.seed(seed);
	random_shuffle(cards.begin(), cards.end(), [&](int i){return e() % i; });

	//����
	int pos = 0;

	//4��6�ŵ�=24
	for (int i = 0; i < 4; ++i)
	{
		vector<Card> deskOne;
		for (int j = 0; j < 6; ++j)
			deskOne.push_back(cards[pos++]);
		poker->desk.push_back(deskOne);
	}

	//6��5�ŵ�=30
	for (int i = 0; i < 6; ++i)
	{
		vector<Card> deskOne;
		for (int j = 0; j < 5; ++j)
			deskOne.push_back(cards[pos++]);
		poker->desk.push_back(deskOne);
	}

	//5�� ������=50
	for (int i = 0; i < 5; ++i)
	{
		vector<Card> cornerOne;
		for (int j = 0; j < 10; ++j)
			cornerOne.push_back(cards[pos++]);
		poker->corner.push_back(cornerOne);
	}

	//ÿ�������������
	for (auto &deskOne : poker->desk)
		deskOne.back().show = true;

	poker->score = 500;
	poker->operation = 0;

	return true;
}

#ifndef _CONSOLE
void Deal::StartAnimation(HWND hWnd,bool &bOnAnimation,bool &bStopAnimation)
{
	//ˢ���Ƶ����λ��
	SendMessage(hWnd, WM_SIZE, 0, 0);

	shared_ptr<SequentialAnimation> seq(make_shared<SequentialAnimation>());
	POINT ptStart = poker->corner.back().back().GetPos();

	vector<AbstractAnimation*> vecFinal;
	for (int i = 0; i < 54; ++i)
	{
		int deskIndex = i % 10;
		int cardIndex = i / 10;

		auto& card = poker->desk[deskIndex][cardIndex];

		//����������Ϊ���ɼ�
		card.SetVisible(false);

		//����������z-index
		seq->Add(new SettingAnimation<Card, int>(&card,0,&Card::SetZIndex,999-i));

		//����������Ϊ�ɼ�
		seq->Add(new SettingAnimation<Card, bool>(&card,0,&Card::SetVisible,true));

		//�������ӽ��䵽ָ��λ��
		seq->Add(new ValueAnimation<Card,POINT>(&card,25,&Card::SetPos,ptStart,card.GetPos()));

		card.SetPos(ptStart);

		//�������ָ�z-index
		seq->Add(new SettingAnimation<Card, int>(&card,0,&Card::SetZIndex,0));

		//����������Ϊ����
		card.show = false;

		//���10����
		if (cardIndex == poker->desk[deskIndex].size() - 1)
		{
			//���淭������ʾ
			vecFinal.push_back(new ValueAnimation<TImage, double>(&card.GetBackImage(),25,&TImage::SetIWidth,1.0,0.0));

			//��������ʾ������
			vecFinal.push_back(new SettingAnimation<Card, bool>(&card,0,&Card::SetShow,true));

			//���淭����
			vecFinal.push_back(new ValueAnimation<TImage, double>(&card.GetImage(),25,&TImage::SetIWidth,0.0,1.0));
		}
	}

	seq->Add(vecFinal);

	//
	int msAll = 25 * 54+50*10 ;
	int times = msAll / 125+1;
	auto play = []()
	{
		PlaySound((LPCSTR)IDR_WAVE_DEAL, GetModuleHandle(NULL), SND_RESOURCE | SND_SYNC);
	};
	while (times--)
	{
		thread t(play);
		t.detach();
	}

	bOnAnimation = true;
	seq->Start(hWnd, bStopAnimation);
	bOnAnimation = false;
}
#endif

bool Deal::Redo(Poker* inpoker)
{
	poker = inpoker;

	poker->desk.clear();
	poker->corner.clear();
	poker->finished.clear();

	return true;
}