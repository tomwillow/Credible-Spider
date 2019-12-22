#pragma once
#include <istream>
#include <memory>
#include <unordered_set>

#include "Poker.h"
#include "Action.h"

class TImage;
class Manager
{
private:
	Poker* poker;
	std::vector<std::shared_ptr<Action>> record;

	void NewGame(std::istream& in);
	void NewGameRandom(std::istream& in);
	void showHelpInfo() const;
	bool Move(Poker* poker, std::istream& in);
	bool CanPick(Poker* poker, std::istream& in);
	void ReleaseRecord();
public:
	Manager();
	Manager(int suitNum);
	Manager(int suitNum, int seed);
	~Manager();

	const Poker* GetPoker() { return poker; }
	bool CanRedo();
	bool Command(const std::string cmd);
	bool readIn(std::istream& in);

	bool AutoSolve();

	//
	TImage *imgCornerDemo;
	std::vector<TImage*> vecImgCardEmpty;
	std::vector<TImage*> vecImgCorner;
	const int border=10;
	const int cardWidth = 71;
	const int cardHeight = 96;
	const int cardGapH = 10;
	void SetImage(int idCardEmpty,int idCardBack,int idCard1);
	void OnSize(RECT rect);
	void Draw(HDC hdc);
	void StartDealAnimation(HWND hWnd);
};


bool dfs(Poker& result, bool& success, int& calc, std::shared_ptr<Poker> poker, std::vector<std::shared_ptr<Action>>& record, std::unordered_set<Poker>& states, int stackLimited, int calcLimited);