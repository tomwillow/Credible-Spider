#pragma once

#include "Action.h"

#include <memory>
#include <windows.h>

class Restore;
class ReleaseCorner:public Action
{
private:
	bool success;
	std::shared_ptr<Restore> restored;

	POINT ptStart;
	std::vector<POINT> vecStartPos, vecEndPos;
public:
	ReleaseCorner() :Action(), success(false), restored(nullptr), ptStart({ 0,0 }) {}

	virtual ~ReleaseCorner() override;

	virtual bool Do(Poker* inpoker) override;
	virtual bool Redo(Poker* inpoker) override;

	std::string GetCommand()const
	{
		return std::string("r");
	}

	virtual void StartAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation) override;
	virtual void RedoAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation) override;
};

