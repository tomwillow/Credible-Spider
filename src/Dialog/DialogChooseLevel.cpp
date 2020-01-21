#include "DialogChooseLevel.h"

#include "Manager.h"
#include "TStatic.h"
#include "DialogSearch.h"
#include <string>
#include <thread>

using namespace std;

LRESULT DialogChooseLevel::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ret = make_shared<ReturnType>();
	ret->isRandom = true;
	ret->suit = 1;
	ret->seed = 0;
	ret->solved = false;

	editSeed.LinkControl(m_hWnd, IDC_EDIT_SEED);
	editSeed.SetEnable(false);

	cbCanBeSolved.LinkControl(m_hWnd, IDC_CHECK_CANBESOLVED);

	rbGroupLevel.LinkControl(m_hWnd, { IDC_RB_LEVEL1,IDC_RB_LEVEL2, IDC_RB_LEVEL4 }, { 1,2,4 });
	rbGroupLevel.SetChecked(IDC_RB_LEVEL1);

	rbGroupSeed.LinkControl(m_hWnd, { IDC_RADIO_RANDOMSEED,IDC_RADIO_INPUTSEED }, { 1,0 });
	rbGroupSeed.SetChecked(IDC_RADIO_RANDOMSEED);

	btnOK.LinkControl(m_hWnd, IDOK);

	btnCancel.LinkControl(m_hWnd, IDCANCEL);

	CenterWindow();
	return TRUE;    // let the system set the focus
}

LRESULT DialogChooseLevel::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ret = nullptr;
	EndDialog(0);
	return 0;
}

LRESULT DialogChooseLevel::OnPropertyChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	WORD& id = wID;
	ret->suit = rbGroupLevel.ReceiveCommand(id);
	ret->isRandom = rbGroupSeed.ReceiveCommand(id);

	if (ret->suit != 4 && ret->isRandom)
	{
		cbCanBeSolved.SetEnable(true);
	}
	else
	{
		cbCanBeSolved.SetEnable(false);
		cbCanBeSolved.SetChecked(false);
	}


	editSeed.SetEnable(ret->isRandom == false);

	return 0;
}

LRESULT DialogChooseLevel::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int id = LOWORD(wParam);

	ret->suit = rbGroupLevel.GetValue();
	ret->isRandom = rbGroupSeed.GetValue();

	switch (id)
	{
	case IDOK:
	{

		if (ret->isRandom == false)
		{
			try
			{
				ret->seed = std::stoul(editSeed.GetText());
			}
			catch (...)
			{
				MessageBox(("无效的种子。范围：0-" + std::to_string(UINT_MAX)).c_str(), "错误", MB_OK | MB_ICONERROR);
				::SetFocus(editSeed.GetHWND());
				break;
			}
		}

		if (cbCanBeSolved.GetChecked())
		{
			DialogSearch dialogSearch(ret->suit);
			dialogSearch.DoModal();
			if (dialogSearch.pSeed)
			{
				//搜索成功
				ret->isRandom = false;
				ret->seed = *dialogSearch.pSeed;
				ret->calc = *dialogSearch.pCalc;
				ret->solved = true;
				EndDialog(0);
			}
			break;
		}
		EndDialog(0);
		break;
	}
	case IDCANCEL:

		SendMessage(WM_CLOSE);
		break;
	}

	return 0;
}