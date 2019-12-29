#include "Card.h"

bool operator==(const Card& lhs, const Card& rhs)
{
	return lhs.suit == rhs.suit && lhs.point == rhs.point && lhs.show == rhs.show;
}

std::ostream& operator<<(std::ostream& out, const Card& card)
{
	out << (card.show ? "" : "[") << card.getSuit() << card.point << (card.show ? "" : "]");
	return out;
}