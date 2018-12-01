#pragma once
#include <istream>

#include "Poker\Poker.h"
#include "Poker\Action.h"
class Manager
{
private:
	std::vector<Action *> record;

	void Manager::showHelpInfo() const;
	void Manager::testMove(Poker *poker, std::istream &in);
	void Manager::ReleaseRecord();
public:
	Poker *poker;
	Manager();
	~Manager();
	void Act()
	{
		for (auto &r : record)
			r->Do(poker);
	}

	void Manager::Command(const std::string cmd);
	void Manager::readIn(std::istream &in);

	void Manager::autoSolve();
};

