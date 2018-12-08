#pragma once

#include "Action.h"

class PMove:public Action
{
private:
	bool bMoveMode;//ÒÆÅÆÄ£Ê½
	int origDesk;
	int start;
	int destDesk;
	bool Move(Poker *poker);
	bool canMove(const Poker *poker, int deskNum, int pos) const;
public:
	PMove(int origDesk, int start, int destDesk) :origDesk(origDesk), start(start), destDesk(destDesk), bMoveMode(true){}
	PMove(int origDesk, int start) :origDesk(origDesk), start(start), bMoveMode(false){}
	bool Do(Poker *poker) override;
	std::string GetCommand()
	{
		using namespace std;
		return string("m ") + to_string(origDesk) + " " + to_string(start) + " " + to_string(destDesk);
	}
};

