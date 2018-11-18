#pragma once

#include "Poker\Poker.h"
#include "Poker\Action.h"
class Manager
{
private:
	Poker *poker;
	std::vector<Action *> record;

	void Manager::showHelpInfo() const;
	void Manager::testMove(Poker *poker);
	void Manager::ReleaseRecord();
public:
	Manager();
	~Manager();
	void Manager::test();
	void Act()
	{
		for (auto &r : record)
			r->Do(poker);
	}
};

