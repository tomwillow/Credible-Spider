#pragma once
#include "AbstractAnimation.h"
#include <chrono>

template <typename TClass,typename TType>
class SettingAnimation :
	public AbstractAnimation
{
private:
	TClass* obj;
	int duration;
	TType value;
	using PSetFunc = void(TClass::*)(TType);
	PSetFunc pSetFunc;
public:
	SettingAnimation(TClass* obj) :obj(obj) {}

	void SetDuration(int ms)
	{
		duration = ms;
	}

	void SetPFunc(PSetFunc pFunc)
	{
		pSetFunc = pFunc;
	}

	void SetValue(TType value)
	{
		this->value = value;
	}

	virtual void Start(HWND hWnd)override
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
		}
		(obj->*pSetFunc)(value);
		InvalidateRect(hWnd, &rect, FALSE);
		UpdateWindow(hWnd);
	}
};

