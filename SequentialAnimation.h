#pragma once
#include "AbstractAnimation.h"
#include <queue>

#include <string>

class SequentialAnimation:public AbstractAnimation
{
private:
	std::queue<AbstractAnimation*> animations;
public:
	void Add(AbstractAnimation* animation)
	{
		animations.push(animation);
	}

	virtual void Start(HWND hWnd)override
	{
		while (!animations.empty())
		{
			animations.front()->Start(hWnd);

			delete animations.front();
			animations.pop();
		}
	}
};

