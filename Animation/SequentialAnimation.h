#pragma once
#include "AbstractAnimation.h"

#include <queue>
#include <vector>

class SequentialAnimation:public AbstractAnimation
{
private:
	std::queue<AbstractAnimation*> animations;
public:
	SequentialAnimation() :AbstractAnimation(0) {}

	void Add(AbstractAnimation* animation)
	{
		animations.push(animation);
	}

	void Add(std::vector<AbstractAnimation*> vecAni)
	{
		for (auto ani : vecAni)
			animations.push(ani);
	}

	virtual void Start(HWND hWnd, bool& StopAnimation)override;
};

