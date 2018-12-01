#pragma once
#include <Windows.h>

class TImage
{
private:
	HBITMAP hBitmapImag;
	BITMAP bitmap;
	LONG cxBitmap, cyBitmap;
	HDC hdcMemImag;
public:
	TImage::TImage(HINSTANCE hInst, int Id);
	~TImage();
	void TImage::Draw(HDC hdc, int dest_x, int dest_y, int img_x = 0, int img_y = 0, DWORD rop = SRCCOPY);
	void TImage::Fill(HDC hdc, const RECT &rect);
};

