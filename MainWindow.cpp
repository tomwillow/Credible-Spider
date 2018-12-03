#include "MainWindow.h"

#include "Dialog/DialogChooseLevel.h"
extern Manager manager;

using namespace std;

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


	hBrushTipBox = CreateSolidBrush(crTipBox);

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

	DeleteObject(hBrushTipBox);
	PostQuitMessage(0);
	return 0;
}

LRESULT MainWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD id = LOWORD(wParam);
	return 0;
}

int GetIndexFromSuitPoint(int suit, int point)
{
	return (suit - 1) * 13 + point - 1;
}

void MainWindow::RefreshTipBoxText()
{
	textTipBox = "分数：" + std::to_string(manager.poker->score) + "\r\n";
	textTipBox += "操作：" + std::to_string(manager.poker->operation);
}

void MainWindow::Draw(HDC hdc, const RECT &rect)
{
	//Background
	imgBackground->Fill(hdc, rect);

	//TipBox

	SelectObject(hdc, hBrushTipBox);
	Rectangle(hdc, rectTipBox.left, rectTipBox.top, rectTipBox.right, rectTipBox.bottom);

	DrawTextCenter(hdc, textTipBox.c_str(), rectTipBox, 12, 400, RGB(255, 255, 255), TEXT("宋体"), DT_LEFT);

	//Card Empty
	if (cardEmpty)
	{
		for (auto &pt : vecDesk)
			imgCardEmpty->Draw(hdc, pt.x, pt.y);
	}

	//Draw desk
	//for (auto &pt : vecCardBack)
	//	imgCardBack->Draw(hdc, pt.x, pt.y);

	//for (auto &card : vecCard)
	//	imgCard[card.first]->Draw(hdc, card.second.x, card.second.y);

	for (auto &vec : vecCard)
	{
		for (auto &card : vec)
		{
			card.first->Draw(hdc, card.second.x, card.second.y);
		}
	}

	//Draw corner
	for (auto &pt : vecCorner)
		imgCardBack->Draw(hdc, pt.x, pt.y);

	if (bTimerDeal)
		imgCard[animateCard.first]->Draw(hdc, animateCard.second.x, animateCard.second.y);
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

	rectTipBox.left = (rect.right - TIPBOX_WIDTH) / 2;
	rectTipBox.right = rectTipBox.left + TIPBOX_WIDTH;
	rectTipBox.bottom = rect.bottom - BORDER;
	rectTipBox.top = rectTipBox.bottom - TIPBOX_HEIGHT;

	//Refresh Corner Card Coordinates
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

	//Card
	int col = 0;
	//vecCard.clear();
	//vecCardBack.clear();
	for (auto &deskCards : manager.poker->desk)
	{
		std::vector < std::pair<TImage*, POINT>> temp;
		int x = vecDesk[col].x;
		int rol = 0;
		for (auto &card : deskCards)
		{
			int y = vecDesk[col].y + rol*cardGapH;
			if (card.show)
			{
				int index = GetIndexFromSuitPoint(card.suit, card.point);
				temp.push_back({ imgCard[index], { x, y } });
				//vecCard.push_back({ index,~bTimerDeal, { x, y } });
			}
			else
			{
				temp.push_back({ imgCardBack, { x, y } });
				//vecCardBack.push_back({ x, y });
			}
			rol++;
		}
		vecCard.push_back(temp);
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
		bTimerDeal = true;

		manager.Command("dr " + std::to_string(suit));
		cardEmpty = false;

		vector<Card> temp;
		for (auto &vec : manager.poker->desk)
		{
			if (!vec.empty())
			{
				temp.push_back(vec.back());
				vec.pop_back();
			}
		}

		SendMessage(WM_SIZE);
		RefreshTipBoxText();

		Invalidate(true);
		for (int i = 0; i < temp.size(); ++i)
		{
			int destX = vecCard[i].back().second.x;
			int destY = vecCard[i].back().second.y+cardGapH;
			int origX = vecCorner.back().x - BORDER;
			int origY = vecCorner.back().y;
			Card card = temp[i];
			for (int f = 0; f < 30; ++f)
			{
				int x = (destX - origX)*f / 30 + origX;
				int y = (destY - origY)*f / 30 + origY;
				int index = GetIndexFromSuitPoint(card.suit, card.point);
				animateCard = { index, { x, y } };
				BOOL b;
				//OnPaint(WM_PAINT, 0, 0, b);
				HDC hdc = GetDC();
				RECT rect;
				GetClientRect(&rect);
				Draw(hdc, rect);
				ReleaseDC(hdc);
				//Invalidate(true);
			}
			manager.poker->desk[i].push_back(card);
			SendMessage(WM_SIZE);
		}

		bTimerDeal = false;

		Invalidate(false);

	}

	return 0;
}