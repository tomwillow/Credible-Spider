#pragma once
#include "Animation.h"
#include "Poker\Card.h"

class Poker;
class Card;
class ASetCard :
	public Animation
{
private:
	CardDrawer *cardDrawer;
	bool show;
public:
	ASetCard::ASetCard(CardDrawer *cardDrawer,bool show) :cardDrawer(cardDrawer),show(show){};
	void ASetCard::Do(HDC hdc);
};

