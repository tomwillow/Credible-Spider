#pragma once
#include "AbstractAnimation.h"

#include "POINT.h"

#include <thread>
#include <chrono>

template <typename TClass, typename TType>
class ValueAnimation :
	public AbstractAnimation
{
private:
	TClass* obj;
	int duration;
	TType vStart, vEnd;
	using PSetFunc = void(TClass::*)(TType);
	PSetFunc pSetFunc;
public:
	ValueAnimation(TClass* obj,int duration,PSetFunc pSetFunc,TType vStart,TType vEnd) :obj(obj),duration(duration), pSetFunc(pSetFunc),vStart(vStart),vEnd(vEnd) {}

	//void SetDuration(int ms)
	//{
	//	duration = ms;
	//}

	//void SetPFunc(PSetFunc pFunc)
	//{
	//	pSetFunc = pFunc;
	//}

	//void SetStartValue(TType vStart)
	//{
	//	this->vStart = vStart;
	//}

	//void SetEndValue(TType vEnd)
	//{
	//	this->vEnd = vEnd;
	//}

	virtual void Start(HWND hWnd, bool& StopAnimation)override
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		using namespace std::chrono;
		auto startTime = high_resolution_clock::now();
		int msPassed = 0;
		TType vNow;
		while (1)
		{
			msPassed = duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count();
			if (msPassed > duration)
				break;

			double percent = double(msPassed) / duration;
			vNow = (vEnd - vStart) * percent + vStart;
			(obj->*pSetFunc)(vNow);
			InvalidateRect(hWnd, &rect, FALSE);
			UpdateWindow(hWnd);

		}
		(obj->*pSetFunc)(vEnd);
		InvalidateRect(hWnd, &rect, FALSE);
		UpdateWindow(hWnd);
	}
};
