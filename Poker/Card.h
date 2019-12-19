#pragma once

#include <string>
#include <iostream>
class Card
{
public:

	int suit;//花色 1 2 3 4

	int point;//点数 1-13

	bool show;//是否已翻开

	//返回花色 C D H S
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

	//打印牌
	//eg. [S1]
	friend std::ostream& operator<<(std::ostream& out, const Card& card);
};

bool operator==(const Card& lhs, const Card& rhs);

std::ostream& operator<<(std::ostream& out, const Card& card);