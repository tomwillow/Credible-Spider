#pragma once
#include <istream>

#include "Poker\Poker.h"
#include "Poker\Action.h"
class Manager
{
private:
	Poker *poker;
	std::vector<Action *> record;

	void Manager::showHelpInfo() const;
	bool Manager::move(Poker *poker, std::istream &in);
	bool Manager::canPick(Poker *poker, std::istream &in);
	void Manager::ReleaseRecord();
public:
	 void (*pCmdFunc)();
	Manager();
	~Manager();
	void Act()
	{
		for (auto &r : record)
			r->Do(poker);
	}

	const Poker* Manager::GetPoker(){ return poker; }
	bool Manager::canRedo();
	Action* Manager::GetLastAct();
	bool Manager::Command(const std::string cmd);
	bool Manager::readIn(std::istream &in);

	void Manager::autoSolve();
};

