#include "TImage.h"

#include <assert.h>

TImage::TImage(HINSTANCE hInst,int id,int maskId):hInst(hInst),id(id),maskId(maskId),pt({0,0}),iWidth(1.0),iHeight(1.0)
{
	hBitmapImag = LoadBitmap(hInst, (LPCSTR)id);
	GetObject(hBitmapImag, sizeof(BITMAP), &bitmap);
	width = bitmap.bmWidth;
	height = bitmap.bmHeight;

	hdcMemImag = CreateCompatibleDC(NULL);
	SelectObject(hdcMemImag, hBitmapImag);

	if (maskId != -1)
	{
		hBitmapMask = LoadBitmap(hInst, (LPCSTR)maskId);
		GetObject(hBitmapMask, sizeof(BITMAP), &bitmapMask);

		hdcMemMask = CreateCompatibleDC(NULL);
		SelectObject(hdcMemMask, hBitmapMask);

		BitBlt(hdcMemImag, 0, 0, width, height, hdcMemMask, 0, 0, SRCAND);//Í¼Æ¬±³¾°±äºÚ

		BitBlt(hdcMemMask, 0, 0, width, height, hdcMemMask, 0, 0, DSTINVERT);//ÑÚÂëÍ¼·´Ïò

	}
}

TImage::TImage(const TImage& img):TImage(img.hInst,img.id,img.maskId)
{

}

TImage::~TImage()
{
	DeleteObject(hBitmapImag);
	DeleteDC(hdcMemImag);

	if (maskId != -1)
	{
		DeleteObject(hBitmapMask);
		DeleteDC(hdcMemMask);
	}
}


void TImage::Draw(HDC hdc)
{
	Draw(hdc, pt.x, pt.y);
}

void TImage::Draw(HDC hdc,int dest_x,int dest_y,int img_x,int img_y,DWORD rop)
{
	int newWidth = width * iWidth;
	int dWidth = width - width * iWidth;
	int newX = dest_x + dWidth / 2;

	if (maskId != -1)
	{
		StretchBlt(hdc, newX, dest_y, newWidth, height * iHeight, hdcMemMask, img_x, img_y, width, height, SRCAND);//ÍÚ¶´
		StretchBlt(hdc, newX, dest_y, newWidth, height * iHeight, hdcMemImag, img_x, img_y, width, height, SRCPAINT);//ÌîÄÚÈÝ
	}
	else
		assert(StretchBlt(hdc, newX, dest_y, newWidth, height * iHeight, hdcMemImag, img_x, img_y, width, height, rop));
}

void TImage::Fill(HDC hdc,const RECT &rect)
{
	for (int x = 0; x < rect.right; x+= width)
		for (int y = 0; y < rect.bottom;y+= height)
			Draw(hdc, x, y);
}