#pragma once

#include "stdafx.h"
#include "TButton.h"
#include "TStatic.h"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "Manager.h"
#include "ReturnType.h"

class DialogEvaluate :public CDialogImpl<DialogEvaluate>
{
private:
	bool openedMsgBox;

	//线程进行标记
	bool onThread;

	//停止线程标记
	bool stop;

	//线程数
	int threadNum;
	int suit;
	std::vector<ReturnType> input;

	TButton btnCancel;
	TStatic staticMemo;
	std::vector<TStatic> vecStatic;

	std::unordered_set<std::shared_ptr<Manager>> managers;
public:
	std::shared_ptr<std::unordered_map<uint32_t, ReturnType>> ret;

	enum { IDD = IDD_DIALOG_SEARCH };

	BEGIN_MSG_MAP(DialogEvaluate)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	DialogEvaluate(int suit,std::vector<ReturnType> input) :suit(suit),input(input),openedMsgBox(false){}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};