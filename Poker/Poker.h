#pragma once

#include <string>
#include <vector>

#include <iostream>
class Card
{
public:
	int suit;
	int point;
	bool show;
	char getSuit() const
	{
		switch (suit)
		{
		case 1:return 'H';
		case 2:return 'S';
		case 3:return 'C';
		case 4:return 'D';
		default:
			throw std::string("Error:'getSuit():' Undefined suit");
		}
	}

	void print()inline const
	{
		std::cout << (show ? "" : "[") << getSuit() << point << (show ? "" : "]");
	}
};

class Action
{

};

class Poker
{
	/*
	桌面上共10摞牌，前4摞6张，后6摞5张，共6*4+5*6=54张

	角落共5叠，每叠10张，共10*5=50张

	总计104张，104=13*8，为两套牌去掉大小王得到

	heart 红桃
	spade 黑桃
	club 梅花
	diamond 方块
	*/
private:
	int seed;
	int suitNum;
	std::vector<std::vector<Card>> desk;//0为最里面
	std::vector<std::vector<Card>> corner;//0为最里面
	std::vector<std::vector<Card>> finished;
public:
	Poker(int suitNum) :suitNum(suitNum){}
	std::vector<Card> genInitCard();

	void printCard(const std::vector<Card> &cards);

	void printCard(const std::vector<std::vector<Card>> &vvcards);

	void printCard();

	void deal();

	void deal(int seed);

	bool releaseCorner()
	{
		if (corner.empty()) return false;
		for (int i = 0; i < 10; ++i)
		{
			corner.back()[i].show = true;
			desk[i].push_back(corner.back()[i]);
		}
		corner.pop_back();
		return true;
	}

	bool canRestore(int deskNum)
	{

	}
};
