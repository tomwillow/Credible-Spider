#pragma once
#include <Windows.h>

class AbstractAnimation
{
public:
	virtual void Start(HWND hWnd,bool &StopAnimation)=0;
};
