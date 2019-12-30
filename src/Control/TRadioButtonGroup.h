#pragma once
#include "TControl.h"
#include "TCheckBox.h"
#include <assert.h>

#include <vector>
#include <unordered_set>
#include <unordered_map>

class TRadioButtonGroup :
	public TControl
{
private:
	struct TRadioButton
	{
		TCheckBox rb;
		int value;
	};
	int nowId, nowValue;
	std::unordered_set<int> uSet;
	std::unordered_map<int, TRadioButton> uMap;
public:
	void LinkControl(HWND hDlg, std::vector<int> id_group, std::vector<int> values);

	int ReceiveCommand(int id);

	void SetChecked(int id);

	int GetValue();

	virtual void SetEnable(bool bEnable) override;
	virtual bool GetEnable() override;
};

