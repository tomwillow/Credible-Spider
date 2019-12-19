#pragma once

#include <iostream>
#include <string>
#include <vector>

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
};

std::ostream& operator<<(std::ostream& out, const Action& action);