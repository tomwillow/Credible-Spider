#include "Restore.h"
#include "Poker.h"

#include <assert.h>
#include <vector>
#include <algorithm>

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

//���ض�Ӧ�ѵ��ܷ����
bool Restore::CanRestore(Poker* poker,int deskNum) const
{
	if (poker->desk[deskNum].empty())
		return false;
	int pos = poker->desk[deskNum].size() - 1;
	int suit = poker->desk[deskNum][pos].suit;

	//i�ǵ���
	for (int i = 1; i <= 13; ++i)
	{
		//�����һ���ƿ�ʼ���������򣬻�ɫһ�� ����Ի���
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
		//���л���

		//�������ƣ��������һ�ŵ���13�ţ�����˳��Ϊ1-13
		vector<Card> temp(poker->desk[deskNum].rbegin(), poker->desk[deskNum].rbegin() + 13);
		poker->finished.push_back(temp);

		//Ԥ�����λ��
		if (poker->hasGUI)
			for_each(temp.begin(), temp.end(), [&oper](const Card& card) {oper.vecStartPt.push_back(card.GetPos()); });

		//ȥ���ƶѵ���13��
		poker->desk[deskNum].erase(poker->desk[deskNum].end() - 13, poker->desk[deskNum].end());

		//�����������
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
		//δָ���������
		//ɨ��ÿ���ѵ�Ѱ���ܻ��յ���
		for (size_t i = 0; i < poker->desk.size(); ++i)
		{
			DoRestore(poker,i);
		}
	else
	{
		//��ָ���������
		int deskIndex = vecOper.front().origDeskIndex;
		vecOper.clear();
		DoRestore(poker,deskIndex);
	}

	return !vecOper.empty();
}

#ifndef _CONSOLE
void Restore::StartAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation)
{
	PlaySound((LPCSTR)IDR_WAVE_DEAL, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	//ˢ���յ�λ��
	SendMessage(hWnd, WM_SIZE, 0, 0);

	SequentialAnimation* seq = new SequentialAnimation;

	//ÿ��������
	for (auto& oper : vecOper)
	{
		//�յ�
		oper.ptEnd = poker->finished.back().back().GetPos();

		//1-13
		for (int i=0;i<poker->finished.back().size();++i)
		{
			auto& card = poker->finished.back()[i];

			//�������
			card.SetPos(oper.vecStartPt[i]);

			//����finishedͼ��˳��Ϊ13-1���˴�z-index����ͼ��˳���Ϊ1-13
			card.SetZIndex(13 - i);

			//�ƶ�
			seq->Add(new ValueAnimation<Card, POINT>(&card, 25, &Card::SetPos, oper.vecStartPt[i], oper.ptEnd));

			//�ָ�z-index
			seq->Add(new SettingAnimation<Card, int>(&card, 0, &Card::SetZIndex, 0));
		}

		if (oper.shownLastCard)
		{
			auto& card = poker->desk[oper.origDeskIndex].back();
			seq->Add(CardTurnOverAnimation::AddBackToFrontAnimation(card));
		}
	}


		bStopAnimation = false;
		bOnAnimation = true;
		seq->Start(hWnd, bStopAnimation);
		delete seq;
		bOnAnimation = false;
}

void Restore::RedoAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation)
{
	//ˢ���յ�λ��
	SendMessage(hWnd, WM_SIZE, 0, 0);

	SequentialAnimation* seq = new SequentialAnimation;

	//ÿ��������
	for (auto& oper : vecOper)
	{

		auto& cards = poker->desk[oper.origDeskIndex];
		int sz = cards.size();

		//�ȸǻ�ȥ
		if (oper.shownLastCard)
		{
			auto& card = cards[sz - 14];
			seq->Add(CardTurnOverAnimation::AddFrontToBackAnimation(card));
		}

		//13-1
		for (int i = sz - 13; i < sz; ++i)
		{
			auto& card = cards[i];

			//�������
			card.SetPos(oper.ptEnd);

			//
			card.SetZIndex(999 - i);

			//�ƶ�
			seq->Add(new ValueAnimation<Card, POINT>(&card, 25, &Card::SetPos,oper.ptEnd ,oper.vecStartPt[i] ));

			//�ָ�z-index
			seq->Add(new SettingAnimation<Card, int>(&card, 0, &Card::SetZIndex, 0));
		}
	}


	bStopAnimation = false;
	bOnAnimation = true;
	seq->Start(hWnd, bStopAnimation);
	delete seq;
	bOnAnimation = false;
}
#endif

bool Restore::Redo(Poker* inpoker)
{
	assert(vecOper.size());

	poker = inpoker;

	for (auto it = vecOper.rbegin(); it != vecOper.rend(); ++it)
	{
		poker->score -= 100;

		//����������򷭻�ȥ
		if (it->shownLastCard)
			poker->desk[it->origDeskIndex].back().show = false;

		//����ɵ��ƷŻضѵ�
		auto it1 = poker->finished.back().rbegin();
		auto it2 = poker->finished.back().rend();
		poker->desk[it->origDeskIndex].insert(poker->desk[it->origDeskIndex].end(), it1, it2);

		//��ɵ�������
		poker->finished.pop_back();
	}

	vecOper.clear();
	return true;
}