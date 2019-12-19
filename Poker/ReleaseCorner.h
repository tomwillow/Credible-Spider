#pragma once

#include "Action.h"

class Restore;
class ReleaseCorner:public Action
{
private:
	bool success;
	Restore* restored;
public:
	ReleaseCorner() :Action(), success(false),restored(nullptr) {}

	virtual ~ReleaseCorner() override;

	virtual bool Do(Poker* inpoker) override;
	virtual bool Redo(Poker* inpoker) override;

	std::string GetCommand()const
	{
		return std::string("r");
	}
};

