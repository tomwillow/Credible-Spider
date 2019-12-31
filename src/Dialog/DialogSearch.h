#pragma once
#include "stdafx.h"
#include "TButton.h"
#include "TStatic.h"

#include <vector>
#include <memory>

class DialogSearch :public CDialogImpl<DialogSearch>
{
private:
	int suit;
	int coreNum;
	bool searchThreadIsRunning;
	bool stopThread;
	std::vector<std::shared_ptr<Manager>> vecManager;
	TButton btnCancel;
	TStatic staticMemo;
	std::vector<TStatic> vecStatic;
public:
	std::shared_ptr<uint32_t> pSeed;

	enum { IDD = IDD_DIALOG_SEARCH };

	BEGIN_MSG_MAP(DialogSearch)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	DialogSearch(int suit) :suit(suit), searchThreadIsRunning(false),stopThread(false){}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	bool IsManagerOnThread();
	void StopManagerThread();
};