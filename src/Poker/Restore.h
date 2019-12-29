#pragma once
#include "Action.h"
#include <vector>
#include <Windows.h>

class Poker;
class Restore :
	public Action
{
private:

	struct Oper
	{
		int origDeskIndex;//回收源堆叠序号
		bool shownLastCard;//回收后底牌是否翻出
		std::vector<POINT> vecStartPt;
		POINT ptEnd;
	};

	//已回收成功的操作
	std::vector<Oper> vecOper;

	//返回对应堆叠能否回收
	bool CanRestore(Poker* poker,int deskNum) const;
	bool DoRestore(Poker* poker,int deskNum);
public:

	//若所有堆叠有可回收的情况则回收
	Restore() : Action(), vecOper() { }

	//若对应堆叠能回收则回收
	Restore(int deskNum) : Action(), vecOper({ { deskNum,false} }) {}

	virtual bool Do(Poker* inpoker) override;
	virtual bool Redo(Poker* inpoker) override;

	std::string GetCommand()const override
	{
		return std::string("restore");
	}

#ifndef _CONSOLE
	virtual void StartAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation) override;
	virtual void RedoAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation) override;
#endif
};

