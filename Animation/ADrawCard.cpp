#include "ADrawCard.h"
#include "TImage.h"

ADrawCard::ADrawCard(TImage *img, POINT pt) :img(img), pt(pt)
{
}

void ADrawCard::Do(HDC hdc)
{
	img->Draw(hdc, pt.x, pt.y);
}