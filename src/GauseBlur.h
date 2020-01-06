#pragma once
#include <Windows.h>

class GauseBlur
{
};


double* CalcGauseBlurMatrix(double sigma, int dist);

UINT GetGauseBlur(int x, int y, const UINT* puStart, int width, const RECT& rect, double* matrix, int dist);