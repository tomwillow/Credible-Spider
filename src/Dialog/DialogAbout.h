#pragma once
#include "stdafx.h"

#include "TEdit.h"
class DialogAbout :public CDialogImpl<DialogAbout>
{
private:
	TEdit edit;
public:

	enum { IDD = IDD_ABOUT };

	BEGIN_MSG_MAP(DialogAuto)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOK)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		EndDialog(0);
		return 0;
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(0);
		return 0;
	}
};