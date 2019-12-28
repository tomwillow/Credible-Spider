#pragma once
#include "stdafx.h"
#include "TImage.h"
#include "Manager.h"

#include <memory>

#define TIMER_DEAL 1
#define TIMER_DEAL_ELPS 10

class MainWindow :public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
private:
	std::shared_ptr<Manager> manager;

	//
	const int cardWidth = 71;
	const int cardHeight = 96;
	const int border = 10;
	std::string textTipBox;
	HBRUSH hBrushTipBox;
	RECT rectTipBox;

	TImage *imgBackground;


	bool bOnDrag;
	POINT ptDragStart, ptChange;
public:
	bool doubleBuffer;
	DECLARE_WND_CLASS(_T("Credible Spider Window"))

	BEGIN_MSG_MAP(MainWindow)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_CLOSE,OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkGnd)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		COMMAND_ID_HANDLER(ID_NEW_GAME, OnNewGame)
		COMMAND_ID_HANDLER(ID_RENEW_GAME, OnReNewGame)
		COMMAND_ID_HANDLER(ID_RELEASE, OnRelease)
		COMMAND_ID_HANDLER(ID_RELEASE2, OnRelease)
		COMMAND_ID_HANDLER(ID_SHOWMOVE, OnShowMove)
		COMMAND_ID_HANDLER(ID_REDO, OnRedo)
		COMMAND_ID_HANDLER(ID_AUTO, OnAuto)
		COMMAND_ID_HANDLER(ID_ABOUT, OnAbout)
	END_MSG_MAP()

	LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void MainWindow::Draw(HDC hdc, const RECT &rect);

	//在多线程动画时，拖动窗口会产生背景擦除，默认情况下会发生白色闪烁。
	//重载WM_ERASEBKGND并返回false可设置为不擦除，消除闪烁
	LRESULT MainWindow::OnEraseBkGnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT MainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MainWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void EnableAllInput(bool enable);
	void MainWindow::RefreshMenu();

	LRESULT MainWindow::OnNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnReNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnRelease(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnShowMove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnRedo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnAuto(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT MainWindow::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};
