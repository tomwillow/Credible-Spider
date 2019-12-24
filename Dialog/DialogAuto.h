#pragma once
#include "stdafx.h"

#include <memory>
#include "Manager.h"
#include "TButton.h"
#include "TCheckBox.h"

class DialogAuto :public CDialogImpl<DialogAuto>
{
private:
	TButton btnStart;
	TCheckBox checkboxAnimation;
	std::shared_ptr<Manager> manager;
public:
	DialogAuto(std::shared_ptr<Manager> manager) :manager(manager) {}

	enum { IDD = IDD_DIALOG_AUTO };

	BEGIN_MSG_MAP(DialogAuto)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_BUTTON_START, OnBtnStart)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnBtnStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};