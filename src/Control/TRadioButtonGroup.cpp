#include "TRadioButtonGroup.h"


void TRadioButtonGroup::LinkControl(HWND hDlg, std::vector<int> id_group, std::vector<int> values)
{
	assert(uSet.empty());
	assert(id_group.size() == values.size());

	for (size_t i = 0; i < id_group.size(); ++i)
	{
		TCheckBox rb;
		uMap[id_group[i]] = { rb,values[i] };
		uMap[id_group[i]].rb.LinkControl(hDlg, id_group[i]);
		uSet.insert(id_group[i]);
	}
}

int TRadioButtonGroup::ReceiveCommand(int id)
{
	if (uSet.find(id) != uSet.end())
	{
		for (auto& pr : uMap)
		{
			pr.second.rb.SetChecked(pr.first == id);
		}
		nowId = id;
		nowValue = uMap[id].value;
		return nowValue;
	}
	else
		return nowValue;
}

void TRadioButtonGroup::SetChecked(int id)
{
	assert(uSet.find(id) != uSet.end());
	ReceiveCommand(id);
}

int TRadioButtonGroup::GetValue()
{
	return nowValue;
}

void TRadioButtonGroup::SetEnable(bool bEnable)
{
	for (auto& pr : uMap)
		pr.second.rb.SetEnable(bEnable);
}
bool TRadioButtonGroup::GetEnable()
{
	
	return (*uMap.begin()).second.rb.GetEnable();
}