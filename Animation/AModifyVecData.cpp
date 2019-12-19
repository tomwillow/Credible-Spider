#include "AModifyVecData.h"


void AModifyVecData::Do(HDC hdc)
{
	switch (mType)
	{
	case Add:
		vecPt->push_back(pt);
		return;
	case Remove:
		vecPt->pop_back();
		return;
	}
}
