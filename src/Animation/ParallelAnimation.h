#pragma once
#include "AbstractAnimation.h"

#include <vector>

class ParallelAnimation :public AbstractAnimation
{
private:
	std::vector<AbstractAnimation*> animations;
public:
	ParallelAnimation() :AbstractAnimation(0) {}
	virtual ~ParallelAnimation();

	void Add(AbstractAnimation* animation);

	virtual void Start(HWND hWnd, bool& StopAnimation)override;
};

