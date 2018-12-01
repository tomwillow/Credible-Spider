#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "Poker/Poker.h"
#include "Manager.h"

#include "MainWindow.h"

CComModule _Module;

Manager manager;

using namespace std;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev,
	LPSTR szCmdLine, int nCmdShow)
{
	InitCommonControls();
	_Module.Init(NULL, hInst);

	MainWindow wndMain;
	MSG msg;

	if (NULL == wndMain.Create(NULL, CWindow::rcDefault,_T("Credible Spider")))
	{
		return 1;
	}

	wndMain.doubleBuffer = true;
	wndMain.ShowWindow(nCmdShow);
	wndMain.UpdateWindow();

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	_Module.Term();
	return msg.wParam;
}

//int main()
//{
//
//	Manager manager;
//	//manager.readIn(cin);
//	manager.readIn(ifstream("1.txt"));
//
//
//	system("pause");
//	return 0;
//}