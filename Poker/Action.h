#pragma once

#include <iostream>
#include <string>

class Poker;
class Action
{
private:
public:
	bool success=false;
	std::string strid;
	virtual void Do(Poker *poker) = 0;
	virtual std::string GetCommand() = 0;
	virtual std::ostream& Output(std::ostream& out)
	{
		return out << GetCommand() << std::endl;
	}
};
