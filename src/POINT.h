#pragma once

#include <Windows.h>

POINT operator+(const POINT& pt1, const POINT& pt2);

POINT operator-(const POINT& pt1, const POINT& pt2);

POINT operator*(const POINT& pt, double d);