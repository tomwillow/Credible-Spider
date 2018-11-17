#include "Poker.h"

#include <iostream>
#include <Windows.h>
#include <random>
#include <algorithm>

using namespace std;

vector<Card> Poker::genInitCard()
{
	vector<Card> result;
	switch (suitNum)
	{
	case 1:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ 2, j });
		break;
	case 2:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ (i>3) ? 1 : 2, j });
		break;
	case 4:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ i % 4 + 1, j });
		break;
	default:
		throw string("Error:'genInitCard(" + to_string(suitNum) + ")");
	}
	return result;
}

void Poker::printCard(const vector<Card> &cards)
{
	for (auto &card : cards)
	{
		card.print();
		cout << " ";
	}
}

void Poker::printCard(const vector<vector<Card>> &vvcards)
{
	for (auto &vcards : vvcards)
	{
		printCard(vcards);
		cout << endl;
	}
}

void Poker::printCard()
{
	for (size_t i = 0; i < desk.size(); ++i)
	{
		cout << "desk " << i << ":" << endl;
		printCard(desk[i]);
		cout << endl << endl;
	}

	for (size_t i = 0; i < corner.size(); ++i)
	{
		cout << "corner " << i << ":" << endl;
		printCard(corner[i]);
		cout << endl << endl;
	}
}

void Poker::deal()
{
	seed = GetTickCount();

	deal(seed);
}

void Poker::deal(int seed)
{
	std::default_random_engine generator;

	//生成整齐牌
	auto cards = genInitCard();

	//打乱
	srand(seed);
	random_shuffle(cards.begin(), cards.end(), [](int i){return rand() % i; });

	//发牌
	int pos = 0;
	for (int i = 0; i < 4; ++i)
	{
		vector<Card> deskOne;
		for (int j = 0; j < 6; ++j)
			deskOne.push_back(cards[pos++]);
		desk.push_back(deskOne);
	}

	for (int i = 0; i < 6; ++i)
	{
		vector<Card> deskOne;
		for (int j = 0; j < 5; ++j)
			deskOne.push_back(cards[pos++]);
		desk.push_back(deskOne);
	}

	for (int i = 0; i < 5; ++i)
	{
		vector<Card> cornerOne;
		for (int j = 0; j < 10; ++j)
			cornerOne.push_back(cards[pos++]);
		corner.push_back(cornerOne);
	}

	//
	for (auto &deskOne : desk)
		deskOne.back().show = true;
}