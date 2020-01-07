#pragma once
#include <Windows.h>

class GauseBlur
{
private:
	int dist;
	double* matrix;
	double* CalcGauseBlurMatrix(double sigma, int dist);
	UINT GetGauseBlur(int x, int y, const UINT* puStart, int width, const RECT& rect, double* matrix, int dist);
public:
	GauseBlur(double sigma, int dist);
	~GauseBlur();

	void Do(UINT* puStart, int width, int height);
};