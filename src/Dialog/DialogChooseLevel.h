#pragma once
#include "stdafx.h"
#include "TCheckBox.h"

class DialogChooseLevel:public CDialogImpl<DialogChooseLevel>
{
private:
	int suit;
	TCheckBox cbCanBeSolved;
public:
	enum { IDD = IDD_CHOOSE_LEVEL };

	BEGIN_MSG_MAP(DialogChooseLevel)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	DialogChooseLevel() :suit(0) {}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		this->CheckRadioButton(IDC_RB_LEVEL1, IDC_RB_LEVEL4, IDC_RB_LEVEL1);
		cbCanBeSolved.LinkControl(m_hWnd, IDC_CHECK_CANBESOLVED);
		suit = 1;
		CenterWindow();
		return TRUE;    // let the system set the focus
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		EndDialog(0);
		return 0;
	}

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_RB_LEVEL1:suit = 1; break;
		case IDC_RB_LEVEL2:suit = 2; break;
		case IDC_RB_LEVEL4:suit = 4; break;
		}

		if (suit == 4)
		{
			cbCanBeSolved.SetEnable(false);
			cbCanBeSolved.SetChecked(false);
		}
		else
		{
			cbCanBeSolved.SetEnable(true);
		}

		switch (id)
		{
		case IDOK:
		{
			int canBeSolved = cbCanBeSolved.GetChecked();


			EndDialog(MAKELONG(suit,canBeSolved ));//low,high
			break;
		}
		case IDCANCEL:
			EndDialog(0);
			break;
		}


		return 0;
	}
};

