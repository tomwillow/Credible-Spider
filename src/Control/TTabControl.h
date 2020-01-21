#pragma once
#include "TControl.h"
#include <vector>
#include <string>
#include <unordered_set>

class TTabControl :
	public TControl
{
private:
	std::unordered_set<TControl*> pAllCtrls;
	std::vector<std::unordered_set<TControl*>> pCtrls;
	virtual LRESULT WndProc(WNDPROC wndproc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)override;
public:
	virtual ~TTabControl();

	void SetRectAsParent();

	void AddTabItem(std::tstring label, std::initializer_list<TControl*> vecp = {});

	void TakeOverControl(int page, std::initializer_list<TControl*> vecp);

	int GetCurSel();

	void SetCurSel(int index);
};

