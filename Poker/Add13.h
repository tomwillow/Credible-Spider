#pragma once

#include "Action.h"

class Add13 :public Action
{
private:
	int deskNum;
public:
	Add13(int deskNum) { strid = "a"; this->deskNum = deskNum; }
	bool Do(Poker *poker);
	std::string GetCommand()
	{
		return std::string("a ") + std::to_string(deskNum);
	}
};