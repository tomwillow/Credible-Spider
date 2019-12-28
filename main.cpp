#include "stdafx.h"

#include "MainWindow.h"
#include "Resource.h"

CComModule _Module;

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


	HACCEL m_hAccelTable = LoadAccelerators(hInst, (LPCTSTR)IDR_ACCELERATOR);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	_Module.Term();
	return msg.wParam;
}