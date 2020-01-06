#define _USE_MATH_DEFINES
#include <math.h>

#include "GauseBlur.h"

double* CalcGauseBlurMatrix(double sigma, int dist)
{
	unsigned int sz = 1 + dist * 2;
	double* ret = new double[sz * sz];
	double sum = 0;
	for (int i = 0; i < sz; ++i)
		for (int j = 0; j < sz; ++j)
		{
			int x = j - dist, y = i - dist;
			ret[i * sz + j] = exp(-(x * x + y * y) / (2.0 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
			sum += ret[i * sz + j];
		}

	for (int i = 0; i < sz; ++i)
		for (int j = 0; j < sz; ++j)
			ret[i * sz + j] /= sum;
	return ret;
}

UINT GetGauseBlur(int x, int y, const UINT* puStart, int width, const RECT& rect, double* matrix, int dist)
{
	UINT ret = 0;
	float r = 0, g = 0, b = 0, alpha = 0, v = 0;
	int sz = 1 + dist * 2;
	int ystart = max(rect.top, y - dist);
	int yend = min(rect.bottom - 1, y + dist);
	int xstart = max(rect.left, x - dist);
	int xend = min(rect.right - 1, x + dist);
	for (int yn = ystart, i = max(0, dist - y); yn <= yend && i < sz; ++yn, ++i)
		for (int xn = xstart, j = max(0, dist - x); xn <= xend && j < sz; ++xn, ++j)
		{
			BYTE* p = (BYTE*)&puStart[yn * width + xn];

			b += matrix[i * sz + j] * p[0];
			g += matrix[i * sz + j] * p[1];
			r += matrix[i * sz + j] * p[2];
			alpha += matrix[i * sz + j] * p[3];
		}
	BYTE* p = (BYTE*)&ret;
	p[0] = b;
	p[1] = g;
	p[2] = r;
	p[3] = alpha;
	return ret;
}