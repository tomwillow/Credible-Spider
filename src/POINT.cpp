#include "POINT.h"

POINT operator+(const POINT& pt1, const POINT& pt2)
{
	return { pt1.x + pt2.x,pt1.y + pt2.y };
}

POINT operator-(const POINT& pt1, const POINT& pt2)
{
	return { pt1.x - pt2.x,pt1.y - pt2.y };
}

POINT operator*(const POINT& pt, double d)
{
	return { LONG(pt.x * d),LONG(pt.y * d) };
}

RECT operator+(const RECT& rc, const POINT& pt)
{
	return { rc.left + pt.x,rc.top + pt.y,rc.right + pt.x,rc.bottom + pt.y };
}

RECT& operator+=(RECT& rc, const POINT& pt)
{
	rc.left += pt.x;
	rc.right += pt.x;
	rc.top += pt.y;
	rc.bottom += pt.y;
	return rc;
}

RECT ExpandRect(const RECT& rc, int d)
{
	return { rc.left - d,rc.top - d,rc.right + d,rc.bottom + d };
}

RECT ExpandRect(const RECT& rc, int dx, int dy)
{
	return { rc.left - dx,rc.top - dy,rc.right + dx,rc.bottom + dy };
}

int GetWidth(const RECT& rc)
{
	return rc.right - rc.left;
}
int GetHeight(const RECT& rc)
{
	return rc.bottom - rc.top;
}

void MoveToZero(RECT& rc)
{
	rc.right -= rc.left;
	rc.bottom -= rc.top;
	rc.left = 0;
	rc.top = 0;
}

void Rectangle(HDC hdc,RECT& rc)
{
	::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}