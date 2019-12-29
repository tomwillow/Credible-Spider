#pragma once
#include <Windows.h>

class TImage
{
private:
	int id,maskId;
	HINSTANCE hInst;

	HBITMAP hBitmapImag,hBitmapMask;
	BITMAP bitmap,bitmapMask;
	HDC hdcMemImag,hdcMemMask;

	LONG width, height;
	double iWidth, iHeight;
	void Draw(HDC hdc, int dest_x, int dest_y, int img_x = 0, int img_y = 0, DWORD rop = SRCCOPY);
public:
	POINT pt;
	TImage(HINSTANCE hInst, int id,int maskId=-1);
	TImage(const TImage& img);
	~TImage();

	void SetIWidth(double iWidth)
	{
		this->iWidth = iWidth;
	}
	void Draw(HDC hdc);
	void Fill(HDC hdc, const RECT &rect);
};

