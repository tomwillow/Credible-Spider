#pragma once
#include "Animation.h"

class TImage;
class ADrawCard :
	public Animation
{
private:
	TImage *img;
	POINT pt;
public:
	ADrawCard(TImage *img,POINT pt);
	void Do(HDC hdc);
};

