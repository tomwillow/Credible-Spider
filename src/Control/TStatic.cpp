#include "TStatic.h"


void TStatic::Create(HWND hParent,RECT rect)
{
	m_hParent = hParent;
	m_hInst = GetModuleHandle(NULL);
	m_hWnd = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE, rect.left,rect.top,rect.right,rect.bottom,
		hParent, 0, m_hInst, 0);
}