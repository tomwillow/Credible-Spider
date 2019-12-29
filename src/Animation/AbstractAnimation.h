#pragma once
#include <Windows.h>

class ParallelAnimation;
class AbstractAnimation
{
protected:
	int duration;
public:
	AbstractAnimation(int duration) :duration(duration) {}
	virtual void Start(HWND hWnd,bool &StopAnimation)=0;

	//并行动画中不应刷新，应交由父容器刷新
	virtual void StartParallel(int ms) {}

	friend class ParallelAnimation;
};

