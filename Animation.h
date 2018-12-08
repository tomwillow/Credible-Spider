#pragma once
#include <Windows.h>
#include <vector>
#include "CardDrawer.h"

class Animation
{
public:
	virtual void Do(HDC hdc) = 0;
};

