#pragma once

#include <string>
#include <iostream>
class Card
{
public:
	int suit;
	int point;
	bool show;
	char getSuit() const
	{
		switch (suit)
		{
		case 1:return 'H';
		case 2:return 'S';
		case 3:return 'C';
		case 4:return 'D';
		default:
			throw std::string("Error:'getSuit():' Undefined suit");
		}
	}

	void print()inline const
	{
		std::cout << (show ? "" : "[") << getSuit() << point << (show ? "" : "]");
	}
};
