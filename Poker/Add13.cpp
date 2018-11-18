#include "Add13.h"

#include "Poker.h"

void Add13::Do(Poker *poker)
{
	if (deskNum >= poker->desk.size()) return;

	for (int i = 13; i >= 1; --i)
		poker->desk[deskNum].push_back({ 1, i, true });

	poker->refresh(deskNum);
	success = true;
}