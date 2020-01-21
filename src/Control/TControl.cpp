#pragma once

#include <stdio.h>//_vsnprintf_s
#include <assert.h>
#include <vector>
#include <tchar.h>

#include "TControl.h"

using namespace std;

TControl::TControl()
{
	m_hWnd = NULL;
	m_hParent = NULL;
	m_hInst = NULL;

	m_hFont = NULL;

	bCanAcceptDrag = false;
}


TControl::~TControl()
{
	if (m_hFont!=NULL)
	::DeleteObject(m_hFont);
}

TControl::TControl(const TControl& control)
{
	*this = control;
}

TControl& TControl::operator=(const TControl& control)
{
	TCHAR className[MAX_PATH];
	GetClassName(control.m_hWnd, className, MAX_PATH);

	LONG style = GetWindowLong(control.m_hWnd, GWL_STYLE);
	LONG exstyle = GetWindowLong(control.m_hWnd, GWL_EXSTYLE);
	LONG id = GetWindowLong(control.m_hWnd, GWL_ID);

	RECT rc = control.GetClientRect();
	RECT rcPos = control.GetPosition();

	m_hParent = control.m_hParent;
	m_hInst = control.m_hInst;
	m_hWnd = CreateWindowEx(exstyle,className, control.GetText().c_str(),
		style,rcPos.left,rcPos.top, rc.right, rc.bottom, 
		control.m_hParent, (HMENU)id, control.m_hInst, 0);
	
	HFONT hFont = (HFONT)SendMessage(control.m_hWnd, WM_GETFONT, 0, 0);
	SetFont(hFont);

	SetDragAccept(control.bCanAcceptDrag);
	return *this;
}


//仅使用x,y坐标，width,height使用原大小
void TControl::SetPositionOnlyOrigin(const RECT &rect)
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	SetPosition(rect.left, rect.top, rc.right, rc.bottom);
}

//right和bottom保存的是宽和高
void TControl::SetRect(RECT &rect)
{
	::MoveWindow(m_hWnd, rect.left,rect.top,  rect.right, rect.bottom, true);
}

//对角点坐标
void TControl::SetRect(int x1, int y1, int x2, int y2)
{
	::MoveWindow(m_hWnd, x1, y1, x2 - x1, y2 - y1, true);
}

//返回：相对于父窗口的 x1, y1, x2, y2
RECT TControl::GetPosition() const
{
	RECT rc = GetWindowRect();
	POINT pt1 = { rc.left,rc.top };
	POINT pt2 = { rc.right,rc.bottom };
	ScreenToClient(m_hParent, &pt1);
	ScreenToClient(m_hParent, &pt2);
	return { pt1.x,pt1.y,pt2.x,pt2.y };
}

void TControl::SetPosition(int x, int y, int width, int height)
{
	::MoveWindow(m_hWnd, x, y, width, height, true);
	//::SetWindowPos(m_hWnd, HWND_TOP, x, y, width, height, 0);//SWP_SHOWWINDOW
}

//rect中各值均为坐标
void TControl::SetPosition(RECT rect)
{
	::MoveWindow(m_hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
	//::SetWindowPos(m_hWnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0);//SWP_SHOWWINDOW
}

vector<tstring> TControl::PreDrop(WPARAM wParam) const
{
	vector<tstring> ret;
	HDROP hDrop = (HDROP)wParam;
	UINT nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); // 拖拽文件个数
	TCHAR strFileName[MAX_PATH];
	for (UINT i = 0; i < nFileNum; i++)
	{
		DragQueryFile(hDrop, i, strFileName, MAX_PATH);//获得拖曳的文件名
		ret.push_back(strFileName);
	}
	DragFinish(hDrop);      //释放hDrop
	return ret;
}

LRESULT TControl::DropProc(const std::vector<tstring>& dropFiles)
{
	return TRUE;
}

LRESULT CALLBACK TControl::subControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TControl * pControl;
#ifdef _WIN64
	pControl = (TControl*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
#else
	pControl = (TControl *)GetWindowLong(hWnd, GWL_USERDATA);
#endif

	WNDPROC oldProc;
	oldProc = (WNDPROC)GetProp(hWnd, TEXT("oldProc"));
	if (pControl)
	{
		if (uMsg == WM_DROPFILES)
		{
			vector<tstring> dropFiles=pControl->PreDrop(wParam);
			return pControl->DropProc(dropFiles);
		}
		else
			return pControl->WndProc((WNDPROC)oldProc, hWnd, uMsg, wParam, lParam);
	}
	else
		return CallWindowProc((WNDPROC)oldProc, hWnd, uMsg, wParam, lParam);
}

void TControl::LinkControl(HWND hDlg, int id)
{
	LinkControl(GetDlgItem(hDlg, id));
}

void TControl::LinkControl(HWND hWndCtrl)//链接到已有控件（用于对话框中）
{
	assert(hWndCtrl);
	m_hInst = GetModuleHandle(NULL);
	m_hParent = GetParent(hWndCtrl);
	m_hWnd = hWndCtrl;

	//测试发现subControlProc中会反复处理 uMsg=0x87, WM_GETDLGCODE 消息，
	//查询 https://blog.csdn.net/amwfnyq/article/details/5612289 描述是控件
	//没有 WS_EX_CONTROLPARENT 样式导致死循环
	//LONG style = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	//SetWindowLong(m_hWnd, GWL_EXSTYLE, style | WS_EX_CONTROLPARENT);

	//再次测试发现是两个HWND注册到同一个class导致死循环

	RegisterProc();
}

void TControl::RegisterProc()//创建窗口后注册
{
#ifdef _WIN64
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	WNDPROC oldProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)subControlProc);
#else
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
	WNDPROC oldProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG_PTR)subControlProc);
#endif
	
	SetProp(m_hWnd, TEXT("oldProc"), oldProc);
}

LRESULT TControl::WndProc(WNDPROC wndproc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)//虚拟消息处理函数，可重载
{
	auto it = msgDealer.find(uMsg);
	if (it != msgDealer.end())
	{
		it->second(hWnd,wParam,lParam);
	}
	return CallWindowProc(wndproc, hWnd, uMsg, wParam, lParam);
}

HWND TControl::GetHWND()
{
	return m_hWnd;
}

void TControl::Invalidate()
{
	InvalidateRect(m_hWnd, &GetClientRect(), FALSE);
}

void TControl::SetFont(HFONT hFont)
{
	SendMessage(m_hWnd,             // Handle of edit control
		WM_SETFONT,         // Message to change the font
		(WPARAM)hFont,     // handle of the font
		MAKELPARAM(TRUE, 0) // Redraw text
		);
}

void TControl::SetDefaultGuiFont()
{
	if (m_hFont != NULL)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	m_hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);//
	SetFont(m_hFont);
}

void TControl::SetFont(TCHAR FontName[], int FontSize)
{
	if (m_hFont != NULL)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	_tcscpy_s(lf.lfFaceName,_tcslen(FontName)+1, FontName);
	lf.lfHeight = -MulDiv(FontSize, GetDeviceCaps(GetDC(m_hWnd), LOGPIXELSY), 72);
	//lf.lfWeight = 900;
	
	m_hFont = CreateFontIndirect(&lf);
	SetFont(m_hFont);
}

void TControl::SetText(const tstring &s)
{
	::SetWindowText(m_hWnd, s.c_str());
}

void TControl::GetText(TCHAR text[])
{
	::GetWindowText(m_hWnd, text, GetLength() + 1);
}

tstring TControl::GetText() const
{
	int len = GetLength()+1;
	TCHAR* temp = new TCHAR[len];
	::GetWindowText(m_hWnd, temp, len);
	tstring s(temp);
	delete[] temp;
	return s;
}

int TControl::GetLength() const
{
	return ::GetWindowTextLength(m_hWnd);
}

void TControl::SetVisible(bool bShow)
{
	::ShowWindow(m_hWnd, bShow ? SW_SHOWNORMAL : SW_HIDE);
}

bool TControl::GetVisible()
{
	return (bool)IsWindowVisible(m_hWnd);
}

void TControl::SetEnable(bool bEnable)
{
	EnableWindow(m_hWnd, bEnable);
}

bool TControl::GetEnable()
{
	return (bool)IsWindowEnabled(m_hWnd);
}

//自动去掉小数末尾0，最多显示6位
void TControl::SetDouble(double d)
{
	tstring s(tto_string(d));
	SetText(s);
}

//获得工具栏大小
RECT TControl::GetClientRect() const
{
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	return rect;
}

RECT TControl::GetWindowRect()const
{
	RECT rect;
	::GetWindowRect(m_hWnd, &rect);
	return rect;
}

void TControl::SetDragAccept(bool bCanAcceptDrop)
{
	this->bCanAcceptDrag = bCanAcceptDrop;
	DragAcceptFiles(m_hWnd, bCanAcceptDrop);
}

void TControl::RegisterMessage(UINT uMsg,PMsgDealer pFunc)
{
	msgDealer[uMsg] = pFunc;
}

void TControl::SetID(int id)
{
	SetWindowLong(m_hWnd, GWL_ID, id);
}