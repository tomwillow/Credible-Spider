#pragma once

#include <string>
#include <iostream>
class Card
{
public:
	int suit;//花色
	int point;
	bool show;
	char getSuit() const
	{
		switch (suit)
		{
		case 1:return 'C';//梅花
		case 2:return 'D';//方块
		case 3:return 'H';//红桃
		case 4:return 'S';//黑桃
		default:
			throw std::string("Error:'getSuit():' Undefined suit");
		}
	}

	void print()inline const
	{
		std::cout << (show ? "" : "[") << getSuit() << point << (show ? "" : "]");
	}
};
