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