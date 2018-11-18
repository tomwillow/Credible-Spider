#include "Poker.h"

#include <iostream>

using namespace std;


void Poker::printCard(const vector<Card> &cards) const
{
	for (auto &card : cards)
	{
		card.print();
		cout << " ";
	}
}

void Poker::printCard(const vector<vector<Card>> &vvcards) const
{
	for (auto &vcards : vvcards)
	{
		printCard(vcards);
		cout << endl;
	}
}

void Poker::printCard() const
{
	for (size_t i = 0; i < desk.size(); ++i)
	{
		cout << "desk " << i << ":";
		printCard(desk[i]);
		cout << endl ;
	}
	cout << endl;

	for (size_t i = 0; i < corner.size(); ++i)
	{
		cout << "corner " << i << ":";
		printCard(corner[i]);
		cout << endl;
	}
	cout << endl;

	for (size_t i = 0; i < finished.size(); ++i)
	{
		cout << "finished " << i << ":";
		printCard(finished[i]);
		cout << endl;
	}
	cout << endl;
}


void Poker::printCard(int deskNum, int pos) const
{
	ISLEGAL(desk,deskNum, pos);
	for (int i = pos; i<desk[deskNum].size(); ++i)
	{
		desk[deskNum][i].print();
		cout << " ";
	}
	cout << endl;
}



//返回对应堆叠能否回收
bool Poker::canRestore(int deskNum) const
{
	int pos = desk[deskNum].size() - 1;
	int suit = desk[deskNum][pos].suit;
	for (int i = 1; i <= 13; ++i)
	{
		//从最后一张牌开始，点数升序，花色一致 则可以回收
		if (pos > 0 && desk[deskNum][pos].point == i && desk[deskNum][pos].suit == suit)
		{
			pos--;
			continue;
		}
		else
			return false;
	}
	return true;
}

void Poker::refresh(int deskNum)
{
	if (canRestore(deskNum))
	{
		//加入套牌
		vector<Card> temp(desk[deskNum].rbegin(), desk[deskNum].rbegin() + 13);
		finished.push_back(temp);

		//去掉原来的一套
		desk[deskNum].erase(desk[deskNum].end() - 13, desk[deskNum].end());

		//翻开下面的牌
		if (!desk[deskNum].empty())
			desk[deskNum].back().show = true;
	}
}

void Poker::refresh()
{
	for (size_t i = 0; i < desk.size();++i)
	{
		refresh(i);
	}
}