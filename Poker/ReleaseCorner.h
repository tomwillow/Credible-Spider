#pragma once

#include "Action.h"

class ReleaseCorner:public Action
{
private:
	bool ReleaseCorner::releaseCorner(Poker *poker);
public:
	ReleaseCorner();
	~ReleaseCorner();
	bool Do(Poker *poker);
	std::string GetCommand()
	{
		return std::string("r");
	}
};

