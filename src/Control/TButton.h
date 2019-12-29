#pragma once
#include "TControl.h"
class TButton :
	public TControl
{
public:
	void CreateButton(HWND hParent, UINT uId, TCHAR text[], HINSTANCE hInst);
};

