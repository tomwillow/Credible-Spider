#pragma once
#include <istream>
#include <memory>
#include <unordered_set>

#include "Poker.h"
#include "Action.h"

extern const std::string AppTitle;

class TImage;
class Manager
{
private:
	Poker* poker;
	std::vector<std::shared_ptr<Action>> record;

	void NewGame(std::istream& in,bool isRandom);
	void NewGameSolved(std::istream& in);
	bool Move(Poker* poker, std::istream& in);
	bool CanPick(Poker* poker, std::istream& in);
	void ReleaseRecord();

	bool AutoSolve(bool playAnimation);

#ifndef _CONSOLE
	bool hasLoadImage;
	HWND hWnd;
	const RECT* pRcClient;
	int soundTip, soundNoTip,soundWin,soundDeal;
	int idCardEmpty, idCardBack, idCard1,idCardMask;
	std::vector<TImage*> vecImgCardEmpty;
	const int border = 10;
	const int xBorder = 15;//已完成牌堆的横向距离
	const int cardWidth = 71;
	const int cardHeight = 96;
	const int cardGapH = 10;

	struct DragInfo
	{
		bool bOnDrag;
		int orig;
		int cardIndex;
		int num;
		std::vector<std::pair<Card*, POINT>> vecCard;
		DragInfo() :bOnDrag(false), orig(-1), cardIndex(-1), num(-1) {}
	} dragInfo;
	void GiveUpDrag();
	POINT GetCardEmptyPoint(RECT rect, int index);
	void GetIndexFromPoint(int& deskIndex, int& cardIndex, POINT pt);
	void InitialImage();
	void RefreshPaint();
#else
	void ShowHelpInfo() const;
#endif
	struct Node
	{
		int value;
		std::shared_ptr<Poker> poker;
		std::shared_ptr<Action> action;
	};

	//emptyIndex传入空数组即可，调用完成会将空牌位索引加入
	//若某一操作后与states中已有的状态重合，则此操作不会加入actions
	std::vector<Manager::Node> GetAllOperator(std::vector<int>& emptyIndex, std::shared_ptr<Poker> poker, const std::unordered_set<Poker>& states);
	bool DFS(bool& success, int& calc, const std::string& origTitle, std::vector<std::shared_ptr<Action>>& record, std::unordered_set<Poker>& states, int stackLimited, int calcLimited, bool playAnimation);
public:
	Manager();
	Manager(int suitNum);
	Manager(int suitNum, uint32_t seed);
	~Manager();

	int GetPokerSuitNum() { return poker->suitNum; };
	int GetPokerOperation() { return poker->operation; };
	int GetPokerScore() { return poker->score; };
	uint32_t GetPokerSeed() { return poker->seed; };
	bool HasPoker() { return poker; };
	bool PokerCornerIsEmpty() { return poker->corner.empty(); }
	//const Poker* GetPoker() { return poker; }

	struct AutoSolveResult
	{
		bool success;
		int calc;
		int suit;
		uint32_t seed;
	};
	AutoSolveResult autoSolveResult;

	//命令：
	//new suit seed
	//newrandom suit
	//auto 显示动画
	bool Command(const std::string cmd);
	bool ReadIn(std::istream& in);
	bool CanRedo();

	//
	bool bOnThread;
	bool bStopThread;
#ifndef _CONSOLE
	void SetSoundId(int idTip,int idNoTip,int idWin,int idDeal);
	void SetTextOutputHWND(HWND hWnd);
	void SetGUIProperty(HWND hWnd,const RECT *rcClient, int idCardEmpty, int idCardBack, int idCard1,int idCardMask);

	void OnSize(const RECT &rect);
	void Draw(HDC hdc,const RECT &rect);

	bool GetIsWon();

	bool ShowOneHint();

	bool OnLButtonDown(POINT pt);
	bool OnLButtonUp(POINT pt);
	bool OnMouseMove(POINT pt);

	//播放胜利音乐，并开一个线程刷新烟花动画
	void Win();

	//返回点是否位于发牌区
	bool PtInRelease(POINT pt);
#endif
};
