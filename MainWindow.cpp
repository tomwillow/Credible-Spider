#include "MainWindow.h"

#include "Dialog/DialogChooseLevel.h"
extern Manager manager;


void DrawTextAdvance(HDC hdc, const TCHAR text[], RECT *rect, long FontSize, int FontWeight, COLORREF color, const TCHAR FontName[], UINT format, int cEscapement=0, int cOrientation=0)
{
	long lfHeight= -MulDiv(FontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT hf= CreateFont(lfHeight, 0, cEscapement, cOrientation, FontWeight, 0, 0, 0, 0, 0, 0, 0, 0, FontName);

	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hf);
	COLORREF crPrev=SetTextColor(hdc, color);
	DrawText(hdc, text, -1, rect, format);
	SetTextColor(hdc, crPrev);
	DeleteObject(hf);
}

void DrawTextCenter(HDC hdc, const TCHAR text[], const RECT &rect, long FontSize, int FontWeight, COLORREF color, const TCHAR FontName[], UINT format)
{
	RECT rectText=rect;
	DrawTextAdvance(hdc, text, &rectText, FontSize, FontWeight, color, FontName, format|DT_CALCRECT);

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
	HMENU hmenu = LoadMenu(_Module.GetResourceInstance(),
		MAKEINTRESOURCE(IDR_MENU_MAIN));
	SetMenu(hmenu);

	HICON hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON));
	SetIcon(hIcon, FALSE);

	imgBackground = new TImage(GetModuleHandle(NULL), IDB_BACKGROUND);
	imgCardEmpty = new TImage(GetModuleHandle(NULL), IDB_CARDEMPTY);
	imgCardBack = new TImage(GetModuleHandle(NULL), IDB_CARDBACK);

	int id_card_first = IDB_CARD1;
	for (int i = 0; i < 52; ++i)
	{
		imgCard[i] = new TImage(GetModuleHandle(NULL), id_card_first + i);
	}

	PostMessage(WM_COMMAND, MAKELONG(ID_NEW_GAME, 0), 0);

	cardEmpty = true;
	return 0;
}

LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	auto answer=MessageBox(TEXT("关闭该游戏之前是否需要保存？"), TEXT("Credible Spider"), MB_YESNOCANCEL|MB_ICONQUESTION);
	switch (answer)
	{
	case IDYES:
	case IDNO:
		DestroyWindow();
	case IDCANCEL:
		break;
	}
		return 0;
}

LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	delete imgBackground;
	delete imgCardEmpty;
	delete imgCardBack;
	for (auto &imgC : imgCard)
		delete imgC;
	PostQuitMessage(0);
	return 0;
}

LRESULT MainWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD id = LOWORD(wParam);
	switch (id)
	{
	case TIMER_DEAL:
	{
		if (timerDealDid1 < 10)
		{

			if (timerDealDid2 < 10)
			{
				timerDealDid2++;
			}
			else
			{
				timerDealDid1++;
				timerDealDid2 = 0;
			}
		}
		else
		{
			KillTimer(TIMER_DEAL);
			bTimerDeal = false;
			//ptAnimateCard.clear();
		}
		Invalidate();
		break;
	}
	}
	return 0;
}

int GetIndexFromSuitPoint(int suit, int point)
{
	return (suit - 1) * 13 + point - 1;
}

void MainWindow::Draw(HDC hdc, const RECT &rect)
{
	//Background
	imgBackground->Fill(hdc, rect);

	//TipBox
	RECT rectTipBox;
	rectTipBox.left = (rect.right - TIPBOX_WIDTH) / 2;
	rectTipBox.right = rectTipBox.left + TIPBOX_WIDTH;
	rectTipBox.bottom = rect.bottom - BORDER;
	rectTipBox.top = rectTipBox.bottom - TIPBOX_HEIGHT;

	HBRUSH hBrush = CreateSolidBrush(crTipBox);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, rectTipBox.left, rectTipBox.top, rectTipBox.right, rectTipBox.bottom);
	DeleteObject(hBrush);

	std::string text;
	text = "分数：" + std::to_string(manager.poker->score) + "\r\n";
	text += "操作：" + std::to_string(manager.poker->operation);
	DrawTextCenter(hdc, text.c_str(), rectTipBox, 12, 400, RGB(255,255,255), TEXT("宋体"), DT_LEFT);

	//Card Empty
	if (cardEmpty)
	{
		for (auto &pt : vecDesk)
			imgCardEmpty->Draw(hdc, pt.x, pt.y);
	}

	//Draw desk
	for (auto &pt : vecCardBack)
		imgCardBack->Draw(hdc, pt.x, pt.y);

	for (auto &card : vecCard)
		imgCard[card.first]->Draw(hdc, card.second.x, card.second.y);

	//Draw corner
	for (auto &pt : vecCorner)
		imgCardBack->Draw(hdc, pt.x, pt.y);
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
	//Refresh Corner Card Coordinates
	RECT rect;
	GetClientRect(&rect);
	vecCorner.resize(manager.poker->corner.size());
	for (int i = 0; i < vecCorner.size(); ++i)
	{
		vecCorner[i].x = rect.right - BORDER - cardWidth - i*BORDER;
		vecCorner[i].y = rect.bottom - BORDER - cardHeight;
	}

	//Card Desk
	vecDesk.resize(10);
	int cardGap = ((rect.right - BORDER * 2) - cardWidth * 10) / 9;
	for (int i = 0; i < 10; ++i)
	{
		vecDesk[i].x = BORDER + i*(cardWidth + cardGap);
		vecDesk[i].y = BORDER;
	}

	int col = 0;
	vecCard.clear();
	vecCardBack.clear();
	for (auto &deskCards : manager.poker->desk)
	{
		int x = vecDesk[col].x;
		int rol = 0;
		for (auto &card : deskCards)
		{
			int y = vecDesk[col].y + rol*cardGapH;
			if (card.show)
			{
				int index = GetIndexFromSuitPoint(card.suit, card.point);
				vecCard.push_back({ index, { x, y } });
			}
			else
				vecCardBack.push_back({ x, y });
			rol++;
		}
		col++;
	}
	return 0;
}

LRESULT MainWindow::OnNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DialogChooseLevel dialogChooseLevel;
	int suit=dialogChooseLevel.DoModal();
	if (suit != 0)
	{
		manager.Command("dr " + std::to_string(suit));
		cardEmpty = false;
		SendMessage(WM_SIZE);
		Invalidate();

		int all_time = 3000;
		int one_time = 3000 / 10;
		int frame_time = one_time / 10;
		for (int i = 0; i < 10; ++i)
		{

			//qAnimate.push({ cardindex[i], true, x, y });
		}
		//qAnimate.push()

		SetTimer(TIMER_DEAL, 50);
		timerDealDid1 = 0;
		timerDealDid2 = 0;
		bTimerDeal = true;
	}

	return 0;
}