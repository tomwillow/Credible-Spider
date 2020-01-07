#pragma once
#include <Windows.h>
#pragma comment(lib,"msimg32.lib")//AlphaBlend

class RectShadow
{
private:
	int width, height;
	RECT rcShadowOut;
	HWND hWnd;
	HDC hdcShadow;
	HBITMAP hBitmapShadow;
public:
	RectShadow(HDC hdc, HWND hWnd, const RECT &rc,int dist,double angleDEG,double sigma);
	~RectShadow();
	void Draw(HDC hdc);
};

