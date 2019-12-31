#pragma once
#include "stdafx.h"
#include "TButton.h"
#include "TEdit.h"
#include "TCheckBox.h"
#include "TRadioButtonGroup.h"

#include <vector>
#include <memory>

class DialogChooseLevel:public CDialogImpl<DialogChooseLevel>
{
private:
	TButton btnOK, btnCancel;
	TEdit editSeed;
	TCheckBox cbCanBeSolved;
	TRadioButtonGroup rbGroupLevel;
	TRadioButtonGroup rbGroupSeed;
public:
	struct DialogChooseLevelReturnType
	{
		bool isRandom;
		int suit;
		uint32_t seed;
	}; 
	std::shared_ptr<DialogChooseLevelReturnType> ret;

	enum { IDD = IDD_CHOOSE_LEVEL };

	BEGIN_MSG_MAP(DialogChooseLevel)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_RB_LEVEL1, OnPropertyChanged)
		COMMAND_ID_HANDLER(IDC_RB_LEVEL2, OnPropertyChanged)
		COMMAND_ID_HANDLER(IDC_RB_LEVEL4, OnPropertyChanged)
		COMMAND_ID_HANDLER(IDC_RADIO_RANDOMSEED, OnPropertyChanged)
		COMMAND_ID_HANDLER(IDC_RADIO_INPUTSEED, OnPropertyChanged)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnPropertyChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

