#pragma once

#include "Card.h"
#include "Action.h"
#include <vector>
#include <Windows.h>

class Deal :public Action
{
private:
	int suitNum;
	uint32_t seed;
	std::vector<Card> genInitCard() const;

	bool enableSound;
	int soundDeal;
public:
	Deal(int suitNum, uint32_t seed,bool enableSound,int soundDeal);

	virtual bool Do(Poker* inpoker) override;
	virtual bool Redo(Poker* inpoker) override;

	virtual std::string GetCommand()const override
	{
		using namespace std;
		return "dr "+to_string(suitNum)+" "+to_string(seed);
	}


#ifndef _CONSOLE
	virtual void StartAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation) override;
	virtual void RedoAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation) override {}
#endif
};

