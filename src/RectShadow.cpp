#define _USE_MATH_DEFINES
#include <math.h>

#include "RectShadow.h"

#include "GauseBlur.h"
#include "POINT.h"

//创建位图，返回HBITMAP，传入指针地址，修改数据可直接影响位图
//来自：为GDI函数增加透明度处理 by启程软件
//https://www.cnblogs.com/setoutsoft/p/4086051.html
HBITMAP CreateGDIBitmap(int nWid, int nHei, void** ppBits)
{
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = nWid;
	bmi.bmiHeader.biHeight = -nHei; // top-down image 
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = 0;

	HDC hdc = GetDC(NULL);
	HBITMAP hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, ppBits, 0, 0);
	ReleaseDC(NULL, hdc);
	return hBmp;
}

RectShadow::RectShadow(HDC hdc,HWND hWnd,const RECT& rc, int d, double angleDEG, double sigma):hWnd(hWnd)
{
	double angleRED = angleDEG / 180.0 * M_PI;
	int dx = d * cos(angleRED);
	int dy = -d * sin(angleRED);

	//计算阴影坐标
	RECT rcShadow = rc + POINT{ dx,dy };
	rcShadowOut = ExpandRect(rcShadow, abs(dx), abs(dy));

	width = GetWidth(rcShadowOut);
	height = GetHeight(rcShadowOut);

	//初始化HDC和HBITMAP
	hdcShadow = CreateCompatibleDC(hdc);
	BYTE* pb = nullptr;
	hBitmapShadow = CreateGDIBitmap(width, height, (void**)&pb);
	SelectObject(hdcShadow, hBitmapShadow);

	//alpha通道设置为0xff
	UINT* pu = (UINT*)pb;
	for (int k = 0; k < width * height; ++k)
	{
		pu[k] = 0xff000000;
	}

	//rcShadow移到画面正中
	MoveToZero(rcShadow);
	rcShadow += POINT{ abs(dx), abs(dy) };
	SelectObject(hdcShadow, GetStockObject(BLACK_BRUSH));

	//GDI函数会将alpha通道置0
	Rectangle(hdcShadow, rcShadow);

	//alpha通道取反
	for (int k = 3; k < width * height * 4; k += 4)
	{
		pb[k] = ~pb[k];
	}

	//对全部像素包括alpha通道做高斯模糊
	GauseBlur gauseBlur(sigma, min(abs(dx),abs(dy)));
	gauseBlur.Do(pu, width, height);
}

RectShadow::~RectShadow()
{
	ReleaseDC(hWnd,hdcShadow);
	DeleteObject(hBitmapShadow);
}

void RectShadow::Draw(HDC hdc)
{
	//以alpha通道进行叠加
	BLENDFUNCTION bf = { AC_SRC_OVER,0,0xff,AC_SRC_ALPHA };
	BOOL bRet = ::AlphaBlend(hdc, rcShadowOut.left, rcShadowOut.top, width, height, hdcShadow, 0, 0, width, height, bf);

	//BitBlt(hdc, rcShadowOut.left, rcShadowOut.top, width, height, hdcShadow, 0, 0, SRCCOPY);
}