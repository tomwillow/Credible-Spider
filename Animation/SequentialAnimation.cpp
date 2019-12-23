#include "SequentialAnimation.h"

void SequentialAnimation::Start(HWND hWnd, bool& StopAnimation)
{
	while (!animations.empty())
	{
		//检测到结束标记
		if (StopAnimation == true)
			break;

		animations.front()->Start(hWnd, StopAnimation);

		delete animations.front();
		animations.pop();
	}

	//进行善后清理
	while (!animations.empty())
	{
		delete animations.front();
		animations.pop();
	}
}