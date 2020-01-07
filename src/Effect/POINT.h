#pragma once

#include <Windows.h>

POINT operator+(const POINT& pt1, const POINT& pt2);

POINT operator-(const POINT& pt1, const POINT& pt2);

POINT operator*(const POINT& pt, double d);

RECT operator+(const RECT& rc, const POINT& pt);

RECT &operator+=(RECT& rc, const POINT& pt);

RECT ExpandRect(const RECT& rc, int d);

RECT ExpandRect(const RECT& rc, int dx,int dy);

int GetWidth(const RECT& rc);
int GetHeight(const RECT& rc);

void MoveToZero(RECT& rc);

void Rectangle(HDC hdc,RECT& rc);