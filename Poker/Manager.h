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

	bool AutoSolve();

	bool hasGUI;
	HWND hWnd;
	int idCardEmpty, idCardBack, idCard1;
	std::vector<TImage*> vecImgCardEmpty;
	const int border = 10;
	const int xBorder = 15;//finished
	const int cardWidth = 71;
	const int cardHeight = 96;
	const int cardGapH = 10;
	void InitialImage();

	struct Node
	{
		int value;
		std::shared_ptr<Poker> poker;
		std::shared_ptr<Action> action;
	};
	void GetAllOperator(std::vector<Manager::Node>& actions, std::vector<int>& emptyIndex, std::shared_ptr<Poker> poker, const std::unordered_set<Poker>& states);
	bool dfs(bool& success, int& calc, std::vector<std::shared_ptr<Action>>& record, std::unordered_set<Poker>& states, int stackLimited, int calcLimited);
public:
	Manager();
	Manager(int suitNum);
	Manager(int suitNum, int seed);
	~Manager();

	const Poker* GetPoker() { return poker; }
	bool CanRedo();
	bool Command(const std::string cmd);
	bool readIn(std::istream& in);

	//
	bool bOnAnimation;
	bool bStopAnimation;
	void SetGUIProperty(HWND hWnd, int idCardEmpty, int idCardBack, int idCard1);
	void OnSize(RECT rect);
	void Draw(HDC hdc);

};
