#pragma once

class ReturnType
{
public:
	bool isRandom;
	int suit;
	unsigned int seed;
	int calc;
	bool solved;
	int reserved;
	ReturnType(bool isRandom, int suit, unsigned int seed, int calc, bool solved, int reserved = 0) :
		isRandom(isRandom), suit(suit), seed(seed), calc(calc), solved(solved), reserved(reserved)
	{

	}

	ReturnType() :ReturnType(false, 0, 0, 0, false, 0) {}
};