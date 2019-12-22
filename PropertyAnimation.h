#pragma once
#include "AbstractAnimation.h"
#include <Windows.h>
#include <chrono>

template <typename TClass>
class PropertyAnimation :
	public AbstractAnimation
{
private:
	TClass* obj;
	int duration;
	POINT ptStart, ptEnd;
	using PSetFunc = void(TClass::*)(POINT);
	PSetFunc pSetFunc;
public:
	PropertyAnimation(TClass* obj) :obj(obj) {}

	void SetDuration(int ms)
	{
		duration = ms;
	}

	void SetPFunc(PSetFunc pFunc)
	{
		pSetFunc = pFunc;
	}

	void SetStartValue(POINT pt)
	{
		ptStart = pt;
	}

	void SetEndValue(POINT pt)
	{
		ptEnd = pt;
	}

	virtual void Start(HWND hWnd)override
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		using namespace std::chrono;
		auto startTime = high_resolution_clock::now();
		int msPassed = 0;
		POINT ptNow;
		//obj->SetZIndex(999);
		while (1)
		{
			msPassed = duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count();
			if (msPassed > duration)
				break;

			double percent = double(msPassed) / duration;
			ptNow = {LONG( (ptEnd.x - ptStart.x) * percent + ptStart.x),
				LONG((ptEnd.y - ptStart.y) * percent + ptStart.y) };
			(obj->*pSetFunc)(ptNow);
			//obj->SetPos(ptNow);
			InvalidateRect(hWnd, &rect, FALSE);
			UpdateWindow(hWnd);
		}
		(obj->*pSetFunc)(ptEnd);
		//obj->SetPos(ptEnd);
		//obj->SetZIndex(0);
		InvalidateRect(hWnd, &rect, FALSE);
		UpdateWindow(hWnd);
	}
};

