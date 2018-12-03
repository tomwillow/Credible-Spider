#include "TImage.h"


TImage::TImage(HINSTANCE hInst,int Id)
{
	hBitmapImag = LoadBitmap(hInst, (LPCSTR)Id);
	GetObject(hBitmapImag, sizeof(BITMAP), &bitmap);
	cxBitmap = bitmap.bmWidth;
	cyBitmap = bitmap.bmHeight;

	hdcMemImag = CreateCompatibleDC(NULL);
}

void TImage::Draw(HDC hdc,int dest_x,int dest_y,int img_x,int img_y,DWORD rop)
{
	SelectObject(hdcMemImag, hBitmapImag);
	BitBlt(hdc, dest_x,dest_y, cxBitmap, cyBitmap, hdcMemImag, img_x,img_y, rop);
}

void TImage::Fill(HDC hdc,const RECT &rect)
{
	for (int x = 0; x < rect.right; x+=cxBitmap)
		for (int y = 0; y < rect.bottom;y+=cyBitmap)
			Draw(hdc, x, y);
}

TImage::~TImage()
{
	DeleteDC(hdcMemImag);
}
