#include "Add13.h"

#include "Poker.h"
#include "Restore.h"
#include <assert.h>

Add13::~Add13()
{
	delete restored;
}

bool Add13::Do(Poker* inpoker)
{
	poker = inpoker;

	assert(deskIndex < poker->desk.size());

	for (int i = 13; i >= 1; --i)
		poker->desk[deskIndex].push_back({ 1, i, true });

	restored = new Restore(deskIndex);
	if (restored->Do(poker))
		;
	else
	{
		delete restored;
		restored = nullptr;
	}

	return true;
}

bool Add13::Redo(Poker* inpoker)
{
	poker = inpoker;

	if (restored)
	{
		restored->Redo(poker);
		delete restored;
		restored = nullptr;
	}

	//²Á³ýÅÆ¶ÑµÄ13ÕÅÅÆ
	poker->desk[deskIndex].erase(poker->desk[deskIndex].end() - 13, poker->desk[deskIndex].end());

	return true;
}