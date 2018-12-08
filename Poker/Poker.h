#pragma once

#include <string>
#include <vector>

#include "Card.h"

#ifdef _DEBUG
#include <assert.h>
#define ISLEGAL(desk,deskNum,pos) assert((pos) >= 0);assert((deskNum)<(desk).size());assert((pos) < (desk)[(deskNum)].size());
#else
#define ISLEGAL(desk,deskNum,pos) ;
#endif

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

	//返回对应堆叠能否回收
	bool canRestore(int deskNum) const;


public:
	int seed;//种子
	int suitNum;//花色
	int score;//分数
	int operation;//操作次数
	bool isFinished();
	std::vector<std::vector<Card>> desk;//0为最里面
	std::vector<std::vector<Card>> corner;//0为最里面
	std::vector<std::vector<Card>> finished;

	void printCard(const std::vector<Card> &cards) const;
	void printCard(const std::vector<std::vector<Card>> &vvcards) const;
	void printCard(int deskNum, int pos) const;
	void printCard() const;

	void refresh();
	void Poker::refresh(int deskNum);


	//void testCanMove() const;

};
