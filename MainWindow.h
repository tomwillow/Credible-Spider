#pragma once
#include "stdafx.h"
#include "TImage.h"

#define TIMER_DEAL 1
#define TIMER_DEAL_ELPS 50

class MainWindow :public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
private:
	bool cardEmpty;
	TImage *imgBackground;
	TImage *imgCardEmpty;
	TImage *imgCardBack;
	TImage *imgCard[52];
	std::vector<POINT> vecDesk;
	//std::vector<POINT> vecCardBack;
	std::vector<std::vector<std::pair<TImage *,POINT>>> vecCard;
	std::vector<POINT> vecCorner;
	std::pair<int, POINT> animateCard;

	std::string textTipBox;
	HBRUSH hBrushTipBox;
	RECT rectTipBox;

	bool bTimerDeal;

public:
	bool doubleBuffer;
	DECLARE_WND_CLASS(_T("Credible Spider Window"))

	BEGIN_MSG_MAP(MainWindow)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_CLOSE,OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		COMMAND_ID_HANDLER(ID_NEW_GAME, OnNewGame)
	END_MSG_MAP()

	LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void MainWindow::Draw(HDC hdc, const RECT &rect);

	LRESULT MainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void MainWindow::RefreshTipBoxText();
	LRESULT MainWindow::OnNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

