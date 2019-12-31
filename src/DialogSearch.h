#pragma once
#include "stdafx.h"
#include "TButton.h"

#include <vector>
#include <memory>

class DialogSearch :public CDialogImpl<DialogSearch>
{
private:
	std::vector<std::shared_ptr<Manager>> vecManager;
	TButton btnCancel;
public:
	struct DialogSearchReturnType
	{
		bool isRandom;
		int suit;
		uint32_t seed;
	};
	DialogSearchReturnType* ret;

	enum { IDD = IDD_CHOOSE_LEVEL };

	BEGIN_MSG_MAP(DialogSearch)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	bool IsManagerOnThread();
	void StopManagerThread();
};

