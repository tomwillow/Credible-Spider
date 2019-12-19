#include "ReleaseCorner.h"

#include "Poker.h"
#include "Restore.h"
#include <assert.h>

using namespace std;

ReleaseCorner::~ReleaseCorner()
{
	delete restored;
}

//释放一摞右下角，没有检查收牌情况
bool ReleaseCorner::Do(Poker* inpoker)
{
	poker = inpoker;

	if (poker->corner.empty())
		return false;

	//遍历一摞待发区牌
	for (int i = 0; i < 10; ++i)
	{
		//待发区亮牌
		poker->corner.back()[i].show = true;

		//逐个堆叠加上
		poker->desk[i].push_back(poker->corner.back()[i]);

	}

	//去掉一摞待发区
	poker->corner.pop_back();
	success = true;

	poker->operation++;

	//刷新看能否回收
	restored = new Restore();
	if (restored->Do(poker))
		;
	else
	{
		delete restored;
		restored = nullptr;
	}

	return true;
}

bool ReleaseCorner::Redo(Poker* inpoker)
{
	assert(success);

	poker = inpoker;

	//
	if (restored)
	{
		restored->Redo(poker);
		delete restored;
		restored = nullptr;
	}

	vector<Card> temp;
	for (int i = 0; i < 10; ++i)
	{
		poker->desk[i].back().show = false;

		temp.push_back(poker->desk[i].back());

		poker->desk[i].pop_back();
	}

	poker->corner.push_back(temp);
	return true;
}