#pragma once

#include <Windows.h>

namespace Draw
{
	void DrawTextCenter(HDC hdc, const TCHAR text[], const RECT& rect, long FontSize, int FontWeight, COLORREF color, const TCHAR FontName[], UINT format);
}

