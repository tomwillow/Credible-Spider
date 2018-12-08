#pragma once

#include <iostream>
#include <string>

class Poker;
class Action
{
private:
public:
	std::string strid;
	virtual bool Do(Poker *poker) = 0;
	virtual std::string GetCommand() = 0;
	virtual std::ostream& Output(std::ostream& out)
	{
		return out << GetCommand() << std::endl;
	}
};
