#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

class Poker;
class Action
{
protected:
	Poker* poker;
public:
	Action():poker(nullptr) {}
	virtual ~Action() {}
	virtual bool Do(Poker* inpoker) = 0;
	virtual bool Redo(Poker* inpoker) = 0;
	virtual std::string GetCommand() const = 0;

	friend std::ostream& operator<<(std::ostream& out, const Action& action);
	virtual void StartAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation)=0;
	virtual void RedoAnimation(HWND hWnd, bool& bOnAnimation, bool& bStopAnimation)=0;
};

std::ostream& operator<<(std::ostream& out, const Action& action);