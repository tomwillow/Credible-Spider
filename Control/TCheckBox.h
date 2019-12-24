#pragma once
#include "TControl.h"
class TCheckBox :
	public TControl
{
private:
	HICON hIconChecked, hIconUnChecked;
	HBITMAP hBitmapChecked, hBitmapUnChecked;
public:
	TCheckBox();
	void SetChecked(bool bChecked);
	void SetCheckedAndBitmap(bool bChecked);
	bool GetChecked();

	void LoadCheckedIcon(HINSTANCE hInst, UINT uiIconId);
	void LoadUnCheckedIcon(HINSTANCE hInst, UINT uiIconId);
	void CreateIconCheckBox(HINSTANCE hInst, HWND hParent, int x, int y,int width,int height, UINT id);
	void SetIconIsChecked();
	void SetIconIsUnChecked();

	void LoadCheckedBitmap(HINSTANCE hInst, UINT uiBitmapId);
	void LoadUnCheckedBitmap(HINSTANCE hInst, UINT uiBitmapId);
	void CreateBitmapCheckBox(HINSTANCE hInst, HWND hParent, int x, int y, int width, int height, UINT id);
	void SetBitmapIsChecked();
	void SetBitmapIsUnChecked();

};

