#include "Firework.h"


std::default_random_engine Firework::e;

HSB operator+(const HSB& l, const HSB& r)
{
	return { l.h + r.h,l.s + r.s,l.b + r.b };
}

HSB operator-(const HSB& l, const HSB& r)
{
	return { l.h - r.h,l.s - r.s,l.b - r.b };
}

HSB operator*(const HSB& l, float f)
{
	return { l.h * f,l.s * f,l.b * f };
}

COLORREF HSB2RGB(HSB hsb) 
{
	COLORREF cr;

	byte* rgb = ((byte*)&cr);
	int hi = int(hsb.h / 60.0) % 6;
	float f = hsb.h / 60.0 - hi;
	unsigned char v = hsb.b * 255.0;
	unsigned char p = hsb.b * (1.0 - hsb.s) * 255.0;
	unsigned char q = hsb.b * (1.0 - f * hsb.s) * 255.0;
	unsigned char t = hsb.b * (1.0 - (1.0 - f) * hsb.s) * 255.0;
	switch (hi)
	{
	case 0:rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
	case 1:rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
	case 2:rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
	case 3:rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
	case 4:rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
	case 5:rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
	}
	rgb[3] = 0;
	return cr;
}