#pragma once
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "tstring.h"

class TControl
{
private:
	bool bCanAcceptDrag;

	using PMsgDealer = LRESULT(*)(HWND hWnd,WPARAM wParam, LPARAM lParam);
	std::unordered_map<UINT,PMsgDealer > msgDealer;

	HFONT m_hFont;
	static LRESULT CALLBACK subControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::vector<std::tstring> PreDrop(WPARAM wParam) const;
public:
	//LONG m_iWidth, m_iHeight;
	TControl();
	virtual ~TControl();

	TControl& operator=(const TControl& control);

	HWND GetHWND();

	void LinkControl(HWND hWndCtrl);
	void LinkControl(HWND hDlg, int id);

	RECT GetClientRect() const;
	RECT GetWindowRect() const;

	void SetRect(RECT &rect);
	void SetRect(int x1, int y1, int x2, int y2); 
	//void SetPos(RECT &rect);
	void Invalidate();

	void SetFont(HFONT hFont);
	void SetDefaultGuiFont();
	void SetFont(TCHAR FontName[], int FontSize);

	void SetText(const std::tstring &s);
	void CDECL SetText(const TCHAR szFormat[], ...);//设置内容
	void GetText(TCHAR text[]);
	TCHAR* GetTCHAR();//返回值由TControl自己负责释放
	std::tstring GetText();
	int GetLength();//获取字符串长度	

	RECT GetPosition() const;
	void SetPosition(int x, int y, int width, int height);//设置大小及位置
	void SetPosition(RECT rect);//设置大小及位置
	void SetPositionOnlyOrigin(const RECT &rect);

	void SetVisible(bool bShow);//设置可见性
	bool GetVisible();

	void SetDouble(double d);
	double GetDouble();

	virtual void SetEnable(bool bEnable);
	virtual bool GetEnable();

	void SetID(int id);

	void SetDragAccept(bool bCanAcceptDrop);

	void RegisterMessage(UINT uMsg,PMsgDealer pFun);
protected:
	HWND m_hParent;
	HWND m_hWnd;
	HINSTANCE m_hInst;
	TCHAR *Text;
	void RegisterProc();//创建窗口后注册

	virtual LRESULT WndProc(WNDPROC wndproc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//虚拟消息处理函数，可覆盖

	virtual LRESULT DropProc(const std::vector<std::tstring>& dropFiles);
};
