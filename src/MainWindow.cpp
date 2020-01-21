#include "MainWindow.h"

#include "Configuration.h"
#include "DialogChooseLevel.h"
#include "DialogHighScore.h"
#include "DialogAuto.h"
#include "DialogAbout.h"

#include "RectShadow.h"
#pragma comment(lib,"winmm.lib")

#include <thread>

using namespace std;

extern Configuration config;
const string savFileName = "credible spider.sav";


LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//加载菜单
	HMENU hmenu = LoadMenu(_Module.GetResourceInstance(),
		MAKEINTRESOURCE(IDR_MENU_MAIN));
	SetMenu(hmenu);

	//加载图标
	HICON hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON));
	SetIcon(hIcon, FALSE);

	//设置背景
	imgBackground = new TImage(GetModuleHandle(NULL), IDB_BACKGROUND);
	hdcBackground = CreateCompatibleDC(NULL);

	//rectShadow = nullptr;

	//初始化manager
	manager = make_shared<Manager>();
	manager->SetSoundId(IDR_WAVE_TIP, IDR_WAVE_NOTIP, IDR_WAVE_SUCCESS, IDR_WAVE_DEAL);
	manager->SetGUIProperty(m_hWnd, &rcClient, IDB_CARDEMPTY, IDB_CARDBACK, IDB_CARD1, IDB_CARDMASK);

	//加载配置
	config.ReadFromFile(savFileName);

	CheckMenuItem(GetMenu(), ID_ENABLE_ANIMATION, config.enableAnimation ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(GetMenu(), ID_ENABLE_SOUND, config.enableSound ? MF_CHECKED : MF_UNCHECKED);

	RefreshMenu();

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
		if (manager->bOnThread)
		{
			//设置停止标记，正在执行的动画检查到标记后做好清理进行退出
			manager->bStopThread = true;

			//设置为不询问，这样下次进入将不询问
			ask = false;

			//再次发送WM_CLOSE消息
			//使用这种方式是因为动画会调用UpdataWindow，如果此处阻塞，正在执行的动画将卡死
			//所以，此处不阻塞，将关闭消息再次加入队列，确保关闭消息始终处于队列中
			//这样，此处将多次到达，并检测动画是否结束
			PostMessage(WM_CLOSE);
			break;
		}
		else
		{
			if (config.SaveToFile(savFileName) == false)
				MessageBox(("保存配置文件 " + savFileName + " 失败.").c_str(), "出错", MB_OK | MB_ICONERROR);
			DestroyWindow();
		}
	}
	case IDCANCEL:
		break;
	}
	return 0;
}

LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	delete imgBackground;

	//delete rectShadow;

	ReleaseDC(hdcMem);

	DeleteDC(hdcBackground);
	PostQuitMessage(0);
	return 0;
}



void MainWindow::Draw(HDC hdc, const RECT& rect)
{
	//贴上背景
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcBackground, 0, 0, SRCCOPY);


	//绘制阴影
	//rectShadow->Draw(hdc);

	//绘制TipBox
	tipBox.Draw(hdc, manager);

	//绘制牌局
	manager->Draw(hdc, rect);
}

LRESULT MainWindow::OnEraseBkGnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//HDC hdc;
	//RECT rect;
	//GetClientRect(&rect);
	//PAINTSTRUCT ps;
	//hdc = BeginPaint(&ps);

	//imgBackground->Fill(hdc, rect);

	//EndPaint(&ps);
	return false;
}

LRESULT MainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef _DEBUG
	//static DWORD prev = 0, now;
	//now = GetTickCount();
	//OutputDebugString((to_string(1000.0/(now - prev))+"\n").c_str());
	//prev = now;
#endif

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(&ps);

	if (doubleBuffer)
	{
		Draw(hdcMem, rcClient);

		BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcMem, 0, 0, SRCCOPY);
	}
	else
		Draw(ps.hdc, rcClient);

	EndPaint(&ps);
	return 0;
}


LRESULT MainWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//因为WM_SIZE会手动发送，所以rcClient不能取用wParam中的值
	GetClientRect(&rcClient);

	tipBox.OnSize(rcClient, border);

	HDC hdc = GetDC();

	ReleaseDC(hdcMem);
	hdcMem = CreateCompatibleDC(hdc);

	//设置hdcMem大小
	HBITMAP hBitmapMem = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	SelectObject(hdcMem, hBitmapMem);
	DeleteObject(hBitmapMem);

	//创建阴影
	//delete rectShadow;
	//rectShadow = new RectShadow(hdc, m_hWnd, rectTipBox, 10, -45, 5.0);

	if (imgBackground)
	{
		ReleaseDC(hdcBackground);
		hdcBackground = CreateCompatibleDC(hdc);

		//设置hdcBackground大小
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
		SelectObject(hdcBackground, hBitmap);
		DeleteObject(hBitmap);

		imgBackground->Fill(hdcBackground, rcClient);

	}
	ReleaseDC(hdc);

	if (manager)
		manager->OnSize(rcClient);

	return 0;
}

LRESULT MainWindow::OnReNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (IDYES == MessageBox("是否重新开始本局游戏？", "询问", MB_YESNO))
	{
		EnableAllInput(false);

		//洗牌
		manager->Command("new " + std::to_string(manager->GetPokerSuitNum()) + " " + std::to_string(manager->GetPokerSeed()));

		EnableAllInput(true);

		RefreshMenu();
	}
	return 0;
}

LRESULT MainWindow::OnNewGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	static bool bOpenDialog = true;
	static ReturnType ret = { 0 };

	if (wNotifyCode==2)
	{
		ReturnType* pret = (ReturnType*)hWndCtl;
		bOpenDialog = false;
		ret = *pret;
		delete pret;
	}

	if (bOpenDialog)
	{
		DialogChooseLevel dialogChooseLevel;
		dialogChooseLevel.DoModal();
		if (dialogChooseLevel.ret)
		{
			ret = *dialogChooseLevel.ret;
		}
		else
			ret.suit = 0;
	}

	if (ret.suit != 0)
	{
		if (manager->bOnThread)
		{
			manager->bStopThread = true;
			bOpenDialog = false;
			PostMessage(WM_COMMAND, MAKELONG(ID_NEW_GAME, 0), 0);
			return 0;
		}

		manager->bStopThread = false;
		bOpenDialog = true;
		auto fun = [&](int suit)
		{
			EnableAllInput(false);


			if (ret.isRandom)
				//随机新游戏
				manager->Command("newrandom " + std::to_string(suit));
			else
				manager->Command("new " + std::to_string(suit) + " " + std::to_string(ret.seed));

			//若发牌途中退出
			if (manager->bStopThread)
				return;

			EnableAllInput(true);

			//
			config.UpdateRecord(manager->GetPokerSuitNum(),manager->GetPokerSeed(),manager->GetPokerScore(),false,ret.calc);

			RefreshMenu();
		};
		thread t(fun, ret.suit);
		t.detach();
	}

	return 0;
}

void MainWindow::RefreshMenu()
{
	if (manager->HasPoker())
	{
		EnableMenuItem(GetMenu(), ID_RENEW_GAME, MF_ENABLED);//重新开始新游戏
	}
	else
	{
		EnableMenuItem(GetMenu(), ID_RENEW_GAME, MF_GRAYED);
	}

	if (manager->HasPoker() && !manager->GetIsWon())
	{
		EnableMenuItem(GetMenu(), ID_AUTO, MF_ENABLED);//启用自动翻牌
		EnableMenuItem(GetMenu(), ID_SHOWMOVE, MF_ENABLED);//启用显示可移动
		EnableMenuItem(GetMenu(), ID_SAVE, MF_ENABLED);//
	}
	else
	{
		EnableMenuItem(GetMenu(), ID_AUTO, MF_GRAYED);
		EnableMenuItem(GetMenu(), ID_SHOWMOVE, MF_GRAYED);
		EnableMenuItem(GetMenu(), ID_SAVE, MF_GRAYED);//
	}

	//刷新 撤销 命令
	if (manager->CanRedo())
		EnableMenuItem(GetMenu(), ID_REDO, MF_ENABLED);//启用撤销
	else
		EnableMenuItem(GetMenu(), ID_REDO, MF_GRAYED);

	//刷新 发牌 命令
	if (manager->HasPoker() && !manager->PokerCornerIsEmpty())
	{
		EnableMenuItem(GetMenu(), ID_RELEASE, MF_ENABLED);//启用发牌
		EnableMenuItem(GetMenu(), ID_RELEASE2, MF_ENABLED);
	}
	else
	{
		EnableMenuItem(GetMenu(), ID_RELEASE, MF_GRAYED);
		EnableMenuItem(GetMenu(), ID_RELEASE2, MF_GRAYED);
	}

	//刷新
	DrawMenuBar();
}

void MainWindow::EnableAllInput(bool enable)
{
	static vector<vector<UINT>> origin;
	if (enable == false)
	{
		//禁用缩放与最大化框
		LONG style = GetWindowLong(GWL_STYLE);
		style ^= WS_THICKFRAME | WS_MAXIMIZEBOX;
		SetWindowLong(GWL_STYLE, style);

		//保存所有菜单项并禁用
		origin.clear();

		HMENU hMenu = GetMenu();
		for (int i = 0; i < GetMenuItemCount(hMenu); ++i)
		{
			vector<UINT> temp;
			HMENU hSubMenu = GetSubMenu(hMenu, i);

			int id = GetMenuItemID(hMenu, i);
			UINT state = GetMenuState(hMenu, id, MF_BYCOMMAND);
			temp.push_back(state);
			EnableMenuItem(hMenu, id, MF_GRAYED);
			for (int j = 0; j < GetMenuItemCount(hSubMenu); ++j)
			{
				int id = GetMenuItemID(hSubMenu, j);

				UINT state = GetMenuState(hMenu, id, MF_BYCOMMAND);
				temp.push_back(state);
				EnableMenuItem(hMenu, id, MF_GRAYED);
			}
			origin.push_back(temp);
		}
		DrawMenuBar();
	}
	else
	{
		//启用缩放与最大化框
		LONG style = GetWindowLong(GWL_STYLE);
		style ^= WS_THICKFRAME | WS_MAXIMIZEBOX;
		SetWindowLong(GWL_STYLE, style);

		//恢复所有菜单项
		HMENU hMenu = GetMenu();
		for (int i = 0; i < GetMenuItemCount(hMenu); ++i)
		{
			HMENU hSubMenu = GetSubMenu(hMenu, i);

			int id = GetMenuItemID(hMenu, i);
			EnableMenuItem(hMenu, id, origin[i][0]);
			for (int j = 0; j < GetMenuItemCount(hSubMenu); ++j)
			{
				int id = GetMenuItemID(hSubMenu, j);

				EnableMenuItem(hMenu, id, origin[i][j + 1]);
			}
		}
		origin.clear();
		DrawMenuBar();
	}
}

LRESULT MainWindow::OnRelease(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (manager->Command("r"))
		RefreshMenu();
	else
	{
		MessageBox("有空位不能发牌。", "提示", MB_OK);
	}
	return 0;
}

LRESULT MainWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptPos;

	ptPos.x = LOWORD(lParam);
	ptPos.y = HIWORD(lParam);

	if (!manager->HasPoker())
		return 0;

	if (manager->bOnThread)
		return 0;

	if (manager->OnLButtonDown(ptPos))
	{
		if (config.enableSound)
			PlaySound((LPCSTR)IDR_WAVE_PICKUP, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	}
	else
	{
		if (PtInRect(&tipBox.GetRect(), ptPos))
			manager->ShowOneHint();
		else
			if (manager->PtInRelease(ptPos))
				OnRelease(0, 0, 0, bHandled);
	}
	return 0;
}

LRESULT MainWindow::OnShowMove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	manager->ShowOneHint();
	return 0;
}

LRESULT MainWindow::OnHighScore(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DialogHighScore dialogHighScore(m_hWnd);
	dialogHighScore.DoModal();
	return 0;
}

LRESULT MainWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (manager->bOnThread)
		return 0;

	POINT ptPos;

	ptPos.x = LOWORD(lParam);
	ptPos.y = HIWORD(lParam);


	if (manager->OnLButtonUp(ptPos))
	{
		if (config.enableSound)
			PlaySound((LPCSTR)IDR_WAVE_PUTDOWN, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

		config.UpdateRecord(manager->GetPokerSuitNum(), manager->GetPokerSeed(), manager->GetPokerScore());

		if (manager->GetIsWon())
		{
			config.UpdateRecord(manager->GetPokerSuitNum(), manager->GetPokerSeed(), manager->GetPokerScore(),true);
			manager->Win();
		}

		RefreshMenu();

	}
	return 0;
}

LRESULT MainWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (manager->bOnThread)
		return 0;

	POINT ptPos;

	ptPos.x = LOWORD(lParam);
	ptPos.y = HIWORD(lParam);


	manager->OnMouseMove(ptPos);

	return 0;
}


LRESULT MainWindow::OnRedo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	manager->Command("redo");

	RefreshMenu();
	return 0;
}

LRESULT MainWindow::OnAuto(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DialogAuto dialogAuto(manager);
	dialogAuto.DoModal();
	auto ret = dialogAuto.ret;

	config.UpdateRecord(manager->GetPokerSuitNum(), manager->GetPokerSeed(), manager->GetPokerScore(),ret->solved,ret->calc);

	if (manager->GetIsWon())
	{
		manager->Win();
	}

	RefreshMenu();

	return 0;
}

LRESULT MainWindow::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DialogAbout dialogAbout;
	dialogAbout.DoModal();

	return 0;
}

LRESULT MainWindow::OnQuit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SendMessage(WM_CLOSE);

	return 0;
}

LRESULT MainWindow::OnSetOption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT state = GetMenuState(GetMenu(), wID, MF_BYCOMMAND);
	if (state & MF_CHECKED)
		state = MF_UNCHECKED;
	else
		state = MF_CHECKED;
	CheckMenuItem(GetMenu(), wID, state);
	switch (wID)
	{
	case ID_ENABLE_ANIMATION:
		config.enableAnimation = state & MF_CHECKED;
		break;
	case ID_ENABLE_SOUND:
		config.enableSound = state & MF_CHECKED;
		break;
	}
	return 0;
}