#pragma once

#include "Card.h"
#include "Action.h"
#include <vector>
#include <Windows.h>

class Deal :public Action
{
private:
	int suitNum;
	int seed;
	std::vector<Card> genInitCard() const;
public:
	Deal(int suitNum) :Action(), suitNum(suitNum),seed(GetTickCount64())
	{
	}

	Deal(int suitNum, int seed) :Action(), suitNum(suitNum), seed(seed)
	{

	}

	virtual bool Do(Poker* inpoker) override;
	virtual bool Redo(Poker* inpoker) override;

	virtual std::string GetCommand()const override
	{
		using namespace std;
		return "dr "+to_string(suitNum)+" "+to_string(seed);
	}

};

