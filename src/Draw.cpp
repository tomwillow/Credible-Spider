#include "Draw.h"

namespace Draw
{

	void DrawTextAdvance(HDC hdc, const TCHAR text[], RECT* rect, long FontSize, int FontWeight, COLORREF color, const TCHAR FontName[], UINT format, int cEscapement = 0, int cOrientation = 0)
	{
		long lfHeight = -MulDiv(FontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		HFONT hf = CreateFont(lfHeight, 0, cEscapement, cOrientation, FontWeight, 0, 0, 0, 0, 0, 0, 0, 0, FontName);

		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, hf);
		COLORREF crPrev = SetTextColor(hdc, color);
		DrawText(hdc, text, -1, rect, format);
		SetTextColor(hdc, crPrev);
		DeleteObject(hf);
	}

	void DrawTextCenter(HDC hdc, const TCHAR text[], const RECT& rect, long FontSize, int FontWeight, COLORREF color, const TCHAR FontName[], UINT format)
	{
		RECT rectText = rect;
		DrawTextAdvance(hdc, text, &rectText, FontSize, FontWeight, color, FontName, format | DT_CALCRECT);

		int width = rectText.right - rectText.left;
		int height = rectText.bottom - rectText.top;

		rectText.left = (rect.right + rect.left) / 2 - width / 2;
		rectText.top = (rect.bottom + rect.top) / 2 - height / 2;
		rectText.right = rectText.left + width;
		rectText.bottom = rectText.top + height;

		DrawTextAdvance(hdc, text, &rectText, FontSize, FontWeight, color, FontName, format);
	}
}