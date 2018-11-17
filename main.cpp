#include <iostream>
#include <vector>

#include "Poker/Poker.h"

using namespace std;

int main()
{
	Poker poker(4);
	poker.deal();
	poker.printCard();

	poker.releaseCorner();
	poker.printCard();

	system("pause");
	return 0;
}