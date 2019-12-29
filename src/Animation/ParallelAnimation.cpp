#include "ParallelAnimation.h"

#include <chrono>
#include <algorithm>

void ParallelAnimation::Add(AbstractAnimation* animation)
{
	duration = max(duration,animation->duration);
	animations.push_back(animation);
}

ParallelAnimation::~ParallelAnimation()
{
	for (auto ani : animations)
		delete ani;
}

void ParallelAnimation::Start(HWND hWnd, bool& StopAnimation)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	using namespace std::chrono;
	auto startTime = high_resolution_clock::now();
	int msPassed = 0;
	while (1)
	{
		msPassed = duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count();
		if (msPassed > duration)
			break;

		for (auto& ani : animations)
			ani->StartParallel(msPassed);
		InvalidateRect(hWnd, &rect, FALSE);
		UpdateWindow(hWnd);
	}
	for (auto& ani : animations)
		ani->StartParallel(ani->duration);
	InvalidateRect(hWnd, &rect, FALSE);
	UpdateWindow(hWnd);
}