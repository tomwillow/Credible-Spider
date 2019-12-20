#pragma once
#include "stdafx.h"
#include "TImage.h"
#include "Animation.h"
#include "Manager.h"

#include <memory>

#define TIMER_DEAL 1
#define TIMER_DEAL_ELPS 10

class MainWindow :public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
private:
	std::shared_ptr<Manager> manager;

	static std::string textTipBox;

	bool cardEmpty;
	TImage *imgBackground;
	TImage *imgCardEmpty;
	TImage *imgCardBack;
	TImage *imgCard[52];
	std::vector<POINT> vecDesk;//∂—µ˛Œª÷√

	std::vector<std::vector<CardDrawer>> vecCard;//ªÊ÷∆≈∆
	static std::vector<POINT> vecCorner;

	HBRUSH hBrushTipBox;
	RECT rectTipBox;

	bool bOnAnimation;

	std::queue<Animation *> qAnimation;

	bool bOnDrag;
	std::vector<std::pair<CardDrawer *,POINT>> dragCard;
	POINT ptDragStart, ptChange;
public:
	bool doubleBuffer;
	static HWND s_hWnd;
	DECLARE_WND_CLASS(_T("Credible Spider Window"))

	BEGIN_MSG_MAP(MainWindow)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_CLOSE,OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		COMMAND_ID_HANDLER(ID_NEW_GAME, OnNewGame)
		COMMAND_ID_HANDLER(ID_RENEW_GAME, OnReNewGame)
		COMMAND_ID_HANDLER(ID_DEAL, OnDeal)
		COMMAND_ID_HANDLER(ID_REDO, OnRedo)
	END_MSG_MAP()

	LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void MainWindow::Draw(HDC hdc, const RECT &rect);

	LRESULT MainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	bool MainWindow::GetPtOnCard(POINT ptMouse, int &col, int &row);
	bool MainWindow::PtInCard(POINT ptMouse,POINT ptCard);

	void MainWindow::AddDealAnimation();
	void MainWindow::RedoDealAnimation();

	void MainWindow::RefreshByManager();
	void MainWindow::RefreshCard();

	void MainWindow::NewGame();
	LRESULT MainWindow::OnNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnReNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnDeal(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnRedo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};
