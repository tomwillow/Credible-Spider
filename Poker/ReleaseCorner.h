#pragma once

#include "Action.h"

#include <windows.h>

class Restore;
class ReleaseCorner:public Action
{
private:
	bool success;
	Restore* restored;
	std::vector<POINT> vecStartPos, vecEndPos;
public:
	ReleaseCorner() :Action(), success(false),restored(nullptr) {}

	virtual ~ReleaseCorner() override;

	virtual bool Do(Poker* inpoker) override;
	virtual bool Redo(Poker* inpoker) override;

	std::string GetCommand()const
	{
		return std::string("r");
	}

	void StartAnimation(HWND hWnd,POINT ptStart, bool& bOnAnimation, bool& bStopAnimation);
	void RedoAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation);
};

