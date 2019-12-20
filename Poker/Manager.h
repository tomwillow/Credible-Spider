#pragma once
#include <istream>
#include <memory>
#include <unordered_set>

#include "Poker.h"
#include "Action.h"
class Manager
{
private:
	Poker *poker;
	std::vector<std::shared_ptr<Action>> record;

	void showHelpInfo() const;
	bool Move(Poker *poker, std::istream &in);
	bool CanPick(Poker *poker, std::istream &in);
	void ReleaseRecord();
public:
	 Manager(int suitNum);
	 Manager(int suitNum,int seed);
	~Manager();

	const Poker* GetPoker(){ return poker; }
	bool CanRedo();
	bool Command(const std::string cmd);
	bool readIn(std::istream &in);

	bool AutoSolve();
};


bool dfs(Poker& result, bool& success, int& calc, std::shared_ptr<Poker> poker, std::vector<std::shared_ptr<Action>>& record, std::unordered_set<Poker>& states, int stackLimited, int calcLimited);