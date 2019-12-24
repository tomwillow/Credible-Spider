#pragma once
#include "stdafx.h"

class DialogChooseLevel:public CDialogImpl<DialogChooseLevel>
{
public:
	enum { IDD = IDD_CHOOSE_LEVEL };

	BEGIN_MSG_MAP(DialogChooseLevel)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOKCancel)
		COMMAND_ID_HANDLER(IDCANCEL, OnOKCancel)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		this->CheckRadioButton(IDC_RB_LEVEL1, IDC_RB_LEVEL4, IDC_RB_LEVEL1);
		CenterWindow();
		return TRUE;    // let the system set the focus
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		EndDialog(0);
		return 0;
	}

	LRESULT OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		int suit = 0;
		if (wID==IDOK)
		{
			if (this->IsDlgButtonChecked(IDC_RB_LEVEL1)) suit = 1;
			if (this->IsDlgButtonChecked(IDC_RB_LEVEL2)) suit = 2;
			if (this->IsDlgButtonChecked(IDC_RB_LEVEL4)) suit = 4;
		}
		EndDialog(suit);
		return suit;
	}
};

