#pragma once
#include <istream>
#include <unordered_set>

#include "Poker.h"
#include "Action.h"
class Manager
{
private:
	Poker *poker;
	std::vector<Action *> record;

	void showHelpInfo() const;
	bool Move(Poker *poker, std::istream &in);
	bool CanPick(Poker *poker, std::istream &in);
	void ReleaseRecord();
public:
	 Manager(int suitNum);
	 Manager(int suitNum,int seed);
	~Manager();

	const Poker* GetPoker(){ return poker; }
	bool canRedo();
	Action* GetLastAct();
	bool Command(const std::string cmd);
	bool readIn(std::istream &in);

	void autoSolve();
};

bool dfs(Poker* poker, std::vector<Action*> record, std::unordered_set<Poker>& states);
