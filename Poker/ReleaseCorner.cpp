#include "ReleaseCorner.h"

#include "Poker.h"

ReleaseCorner::ReleaseCorner() 
{
	success = false;
}


ReleaseCorner::~ReleaseCorner()
{
}

bool ReleaseCorner::releaseCorner(Poker *poker)
{
	if (poker->corner.empty()) return false;

	//遍历10堆牌
	for (int i = 0; i < 10; ++i)
	{
		//亮牌
		poker->corner.back()[i].show = true;

		//发牌
		poker->desk[i].push_back(poker->corner.back()[i]);

		//刷新看能否回收
		poker->refresh(i);
	}
	poker->corner.pop_back();
	return true;
}

//释放一摞右下角，没有检查收牌情况
void ReleaseCorner::Do(Poker *poker)
{
	success = releaseCorner(poker);
}