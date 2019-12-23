#pragma once
#include "AbstractAnimation.h"

#include <queue>

class SequentialAnimation:public AbstractAnimation
{
private:
	std::queue<AbstractAnimation*> animations;
public:
	void Add(AbstractAnimation* animation)
	{
		animations.push(animation);
	}

	virtual void Start(HWND hWnd, bool& StopAnimation)override;
};

