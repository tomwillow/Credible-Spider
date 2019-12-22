#include "TImage.h"


TImage::TImage(HINSTANCE hInst,int Id):hInst(hInst),id(Id),pt({0,0}),iWidth(1.0),iHeight(1.0)
{
	hBitmapImag = LoadBitmap(hInst, (LPCSTR)Id);
	GetObject(hBitmapImag, sizeof(BITMAP), &bitmap);
	width = bitmap.bmWidth;
	height = bitmap.bmHeight;

	hdcMemImag = CreateCompatibleDC(NULL);
}

TImage::TImage(const TImage& img):TImage(img.hInst,img.id)
{

}

TImage::~TImage()
{
	DeleteObject(hBitmapImag);
	DeleteDC(hdcMemImag);
}


void TImage::Draw(HDC hdc)
{
	Draw(hdc, pt.x, pt.y);
}

void TImage::Draw(HDC hdc,int dest_x,int dest_y,int img_x,int img_y,DWORD rop)
{
	SelectObject(hdcMemImag, hBitmapImag);
	int newWidth = width * iWidth;
	int dWidth = width - width * iWidth;
	int newX = dest_x + dWidth / 2;

	StretchBlt(hdc, newX, dest_y, newWidth, height*iHeight, hdcMemImag, img_x, img_y, width, height, rop);
	//BitBlt(hdc, dest_x,dest_y, cxBitmap*iWidth, cyBitmap*iHeight, hdcMemImag, img_x,img_y, rop);
}

void TImage::Fill(HDC hdc,const RECT &rect)
{
	for (int x = 0; x < rect.right; x+= width)
		for (int y = 0; y < rect.bottom;y+= height)
			Draw(hdc, x, y);
}