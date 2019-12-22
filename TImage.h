#pragma once
#include <Windows.h>

class TImage
{
private:
	int id;
	HINSTANCE hInst;
	HBITMAP hBitmapImag;
	BITMAP bitmap;
	LONG width, height;
	double iWidth, iHeight;
	HDC hdcMemImag;
	void Draw(HDC hdc, int dest_x, int dest_y, int img_x = 0, int img_y = 0, DWORD rop = SRCCOPY);
public:
	POINT pt;
	TImage(HINSTANCE hInst, int Id);
	TImage(const TImage& img);
	~TImage();

	void SetIWidth(double iWidth)
	{
		this->iWidth = iWidth;
	}
	void Draw(HDC hdc);
	void Fill(HDC hdc, const RECT &rect);
};

