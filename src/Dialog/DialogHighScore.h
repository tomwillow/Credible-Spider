#pragma once

#include "ReturnType.h"

#include "stdafx.h"

#include "Configuration.h"
#include "TStatic.h"
#include "TListView.h"
#include "TTabControl.h"

class DialogHighScore :public CDialogImpl<DialogHighScore>
{
private:
	HWND hParent;
	std::vector<TListView> vecListView;
	std::vector<TStatic> vecStaticHighScore;
	std::vector<TStatic> vecStaticWinNum;
	TTabControl tabControl;

	int GetSuit(int page);
	void UpdateListViewAndStatic();
	void FillListView(TListView& listView, TStatic& staticHighScore, TStatic& staticWinNum, const std::vector<std::shared_ptr<Configuration::Record>>& record);
public:
	DialogHighScore(HWND hParent) :hParent(hParent) {}

	enum { IDD = IDD_HIGHSCORE };

	BEGIN_MSG_MAP(DialogHighScore)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDC_REPLAY, OnReplay)
		COMMAND_ID_HANDLER(IDC_EVALUATE, OnEvaluate)
		COMMAND_ID_HANDLER(IDC_DELETEALL, OnDeleteAll)
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

	LRESULT OnReplay(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEvaluate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDeleteAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};