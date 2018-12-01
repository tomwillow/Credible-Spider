#pragma once

#include "Card.h"
#include "Action.h"
#include <vector>

class Deal :public Action
{
private:
	bool isRand;
	int suitNum;
	int seed;
	std::vector<Card> Deal::genInitCard() const;
public:
	void set(bool isRand){ this->isRand = isRand; }
	Deal(int suitNum) :suitNum(suitNum), isRand(true){}
	Deal(int suitNum, int seed) :suitNum(suitNum), seed(seed), isRand(false){}
	void Do(Poker *poker);
	std::string GetCommand()
	{
		using namespace std;
		if (isRand)
			return string("dr ") + to_string(suitNum);
		else
			return string("d ") + to_string(suitNum) + " " + to_string(seed);
	}

};

