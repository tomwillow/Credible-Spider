#include "MainWindow.h"

#include "Dialog/DialogChooseLevel.h"
#pragma comment(lib,"winmm.lib")

#include <thread>
using namespace std;

void DrawTextAdvance(HDC hdc, const TCHAR text[], RECT *rect, long FontSize, int FontWeight, COLORREF color, const TCHAR FontName[], UINT format, int cEscapement = 0, int cOrientation = 0)
{
	long lfHeight = -MulDiv(FontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT hf = CreateFont(lfHeight, 0, cEscapement, cOrientation, FontWeight, 0, 0, 0, 0, 0, 0, 0, 0, FontName);

	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hf);
	COLORREF crPrev = SetTextColor(hdc, color);
	DrawText(hdc, text, -1, rect, format);
	SetTextColor(hdc, crPrev);
	DeleteObject(hf);
}

void DrawTextCenter(HDC hdc, const TCHAR text[], const RECT &rect, long FontSize, int FontWeight, COLORREF color, const TCHAR FontName[], UINT format)
{
	RECT rectText = rect;
	DrawTextAdvance(hdc, text, &rectText, FontSize, FontWeight, color, FontName, format | DT_CALCRECT);

	int width = rectText.right - rectText.left;
	int height = rectText.bottom - rectText.top;

	rectText.left = (rect.right + rect.left) / 2 - width / 2;
	rectText.top = (rect.bottom + rect.top) / 2 - height / 2;
	rectText.right = rectText.left + width;
	rectText.bottom = rectText.top + height;

	DrawTextAdvance(hdc, text, &rectText, FontSize, FontWeight, color, FontName, format);
}


LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//加载菜单
	HMENU hmenu = LoadMenu(_Module.GetResourceInstance(),
		MAKEINTRESOURCE(IDR_MENU_MAIN));
	SetMenu(hmenu);

	//加载图标
	HICON hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON));
	SetIcon(hIcon, FALSE);

	imgBackground = new TImage(GetModuleHandle(NULL), IDB_BACKGROUND);

	manager = make_shared<Manager>();
	manager->SetGUIProperty(m_hWnd, IDB_CARDEMPTY, IDB_CARDBACK, IDB_CARD1);

	hBrushTipBox = CreateSolidBrush(crTipBox);


	bOnDrag = false;


	PostMessage(WM_COMMAND, MAKELONG(ID_NEW_GAME, 0), 0);
	return 0;
}

LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	static bool ask = true;
	int answer;
	if (ask)
		answer = MessageBox(TEXT("关闭该游戏之前是否需要保存？"), TEXT("Credible Spider"), MB_YESNOCANCEL | MB_ICONQUESTION);
	else
		answer = IDNO;
	switch (answer)
	{
	case IDYES:
	case IDNO:
	{
		//如果manager正在动画
		if (manager->bOnAnimation)
		{
			//设置停止标记，正在执行的动画检查到标记后做好清理进行退出
			manager->bStopAnimation = true;

			//设置为不询问，这样下次进入将不询问
			ask = false;

			//再次发送WM_CLOSE消息
			//使用这种方式是因为动画会调用UpdataWindow，如果此处阻塞，正在执行的动画将卡死
			//此处不阻塞，将关闭消息再次加入队列，确保关闭消息始终处于队列中
			//这样，此处将多次到达，并检测动画是否结束
			PostMessage(WM_CLOSE);
			break;
		}
		else
			DestroyWindow();
	}
	case IDCANCEL:
		break;
	}
	return 0;
}

LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	delete imgBackground;

	DeleteObject(hBrushTipBox);
	PostQuitMessage(0);
	return 0;
}

void PlaySoundDeal(void *p)
{
		PlaySound((LPCSTR)IDR_WAVE_DEAL, GetInstanceModule(NULL), SND_RESOURCE | SND_SYNC);
}

void MainWindow::Draw(HDC hdc, const RECT &rect)
{
	//Background
	imgBackground->Fill(hdc, rect);

	//TipBox
	if (manager->GetPoker())
	{
		textTipBox = "分数：" + std::to_string(manager->GetPoker()->score) + "\r\n";
		textTipBox += "操作：" + std::to_string(manager->GetPoker()->operation);
	}

	SelectObject(hdc, hBrushTipBox);
	Rectangle(hdc, rectTipBox.left, rectTipBox.top, rectTipBox.right, rectTipBox.bottom);

	DrawTextCenter(hdc, textTipBox.c_str(), rectTipBox, 12, 400, RGB(255, 255, 255), TEXT("宋体"), DT_LEFT);

	//
	manager->Draw(hdc);


}

LRESULT MainWindow::OnEraseBkGnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return false;
}

LRESULT MainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;
	hdc = BeginPaint(&ps);
	GetClientRect(&rect);

	if (doubleBuffer)
	{
		HDC hDCMem;
		HBITMAP hBitmap;
		hDCMem = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		SelectObject(hDCMem, hBitmap);

		Draw(hDCMem, rect);

		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDCMem, 0, 0, SRCCOPY);
		DeleteObject(hBitmap);
		DeleteDC(hDCMem);
	}
	else
		Draw(ps.hdc, rect);

	EndPaint(&ps);
	return 0;
}


LRESULT MainWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rect;
	GetClientRect(&rect);

	//刷新提示框位置
	rectTipBox.left = (rect.right - TIPBOX_WIDTH) / 2;
	rectTipBox.right = rectTipBox.left + TIPBOX_WIDTH;
	rectTipBox.bottom = rect.bottom - border;
	rectTipBox.top = rectTipBox.bottom - TIPBOX_HEIGHT;

	if (manager)
		manager->OnSize(rect);

	return 0;
}

LRESULT MainWindow::OnReNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (IDYES == MessageBox("是否重新开始本局游戏？", "询问", MB_YESNO))
	{
		//洗牌
		manager->Command("new " + std::to_string(manager->GetPoker()->suitNum) + " " + std::to_string(manager->GetPoker()->seed));

		RefreshMenuAndTipBox();
	}
	return 0;
}

LRESULT MainWindow::OnNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DialogChooseLevel dialogChooseLevel;
	int suit = dialogChooseLevel.DoModal();
	if (suit != 0)
	{
		//随机新游戏
		manager->Command("newrandom " + std::to_string(suit));


		//PlaySound((LPCSTR)IDR_WAVE_DEAL, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

		RefreshMenuAndTipBox();
	}

	return 0;
}

void MainWindow::RefreshMenuAndTipBox()
{
	if (manager->GetPoker())
	{
		//重新开始新游戏选项可见
		EnableMenuItem(GetMenu(), ID_RENEW_GAME, MF_ENABLED);
	}
	else
	{
		EnableMenuItem(GetMenu(), ID_RENEW_GAME, MF_DISABLED);
	}

	//刷新 撤销 命令
	if (manager->CanRedo())
		EnableMenuItem(::GetMenu(m_hWnd), ID_REDO, MF_ENABLED);
	else
		EnableMenuItem(::GetMenu(m_hWnd), ID_REDO, MF_DISABLED);

	//刷新 发牌 命令
	if (manager->GetPoker() && manager->GetPoker()->corner.empty())
	{
		EnableMenuItem(::GetMenu(m_hWnd), ID_RELEASE, MF_DISABLED);
	}
	else
		EnableMenuItem(::GetMenu(m_hWnd), ID_RELEASE, MF_ENABLED);


}

LRESULT MainWindow::OnRelease(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	manager->Command("r");

	RefreshMenuAndTipBox();

	return 0;
}

LRESULT MainWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptPos;

	ptPos.x = LOWORD(lParam);
	ptPos.y = HIWORD(lParam);

		////发牌
		//if (!vecCorner.empty())
		//{
		//	//得到牌堆顶端牌RECT
		//	RECT rectCorner;
		//	rectCorner.left = vecCorner.back().x;
		//	rectCorner.top = vecCorner.back().y;
		//	rectCorner.right = rectCorner.left + cardWidth;
		//	rectCorner.bottom = rectCorner.top + cardHeight;

		//	//发牌
		//	BOOL b;
		//	if (PtInRect(&rectCorner, ptPos))
		//		OnDeal(NULL, NULL, NULL, b);
		//}

		////提示
		//if (PtInRect(&rectTipBox, ptPos))
		//{
		//	PlaySound(LPCSTR(IDR_WAVE_TIP), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		//}

		////移牌
		//int col, row;
		//if (GetPtOnCard(ptPos, col, row))
		//{
		//	if (manager.Command("pick " + to_string(col) + " " + to_string(row)))
		//	{
		//		bOnDrag = true;
		//		dragCard.clear();

		//		for (int j = row; j < vecCard[col].size(); ++j)
		//		{
		//			dragCard.push_back({ &vecCard[col][j], vecCard[col][j].pt });
		//		}

		//		ptDragStart = ptPos;
		//		ptChange = { 0, 0 };
		//		PlaySound((LPCSTR)IDR_WAVE_PICKUP, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		//	}
		//}

	return 0;
}

LRESULT MainWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bOnDrag = false;
	return 0;
}

bool MainWindow::PtInCard(POINT ptMouse, POINT ptCard)
{
	return (ptMouse.x >= ptCard.x && ptMouse.x <= ptCard.x + cardWidth &&
		ptMouse.y >= ptCard.y && ptMouse.y <= ptCard.y + cardHeight);
}

bool MainWindow::GetPtOnCard(POINT ptMouse, int &col, int &row)
{
	col = -1, row = -1;
	//for (auto it1 = vecCard.begin(); it1 != vecCard.end(); ++it1)
	//{
	//	for (auto it = it1->rbegin(); it != it1->rend(); ++it)
	//	{
	//		if (it->show && it->img != imgCardBack && PtInCard(ptMouse, it->pt))
	//		{
	//			col = it1 - vecCard.begin();
	//			row = it1->size() - 1 - (it - it1->rbegin());
	//			break;
	//		}
	//	}
	//}
	return (col != -1 && row != -1);
}

LRESULT MainWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptPos;

	ptPos.x = LOWORD(lParam);
	ptPos.y = HIWORD(lParam);
	//string s="";
	//int col = -1, row = -1;
	//GetPtOnCard(ptPos, col, row);
	//s = to_string(col) + "," + to_string(row);

	//if (bOnDrag)
	//{
	//	ptChange.x = ptPos.x - ptDragStart.x;
	//	ptChange.y = ptPos.y - ptDragStart.y;

	//	s = to_string(ptChange.x) + "," + to_string(ptChange.y);
	//	for (auto &pCard : dragCard)
	//	{
	//		pCard.first->pt.x = pCard.second.x + ptChange.x;
	//		pCard.first->pt.y = pCard.second.y + ptChange.y;
	//	}
	//	Invalidate(false);
	//}

	//SetWindowText(s.c_str());
	return 0;
}


LRESULT MainWindow::OnRedo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	manager->Command("redo");

	RefreshMenuAndTipBox();

	return 0;
}

LRESULT MainWindow::OnAuto(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	manager->Command("auto");

	RefreshMenuAndTipBox();

	return 0;
}