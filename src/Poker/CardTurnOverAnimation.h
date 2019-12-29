#pragma once
#include "Card.h"
#include "SequentialAnimation.h"

#include <vector>

namespace CardTurnOverAnimation
{
	std::vector<AbstractAnimation*> AddBackToFrontAnimation(Card& card);

	std::vector<AbstractAnimation*> AddFrontToBackAnimation(Card& card);
}