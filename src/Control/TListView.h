#pragma once
#include <Windows.h>

#include <vector>
#include <string>
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

#include "TControl.h"

class TListView:public TControl
{
private:
public:
	TListView();
	TListView(const TListView& listView);
	TListView& operator=(const TListView& listView);

//创建ListView
	void TListView::CreateListViewEx(HWND hParent, UINT id, HINSTANCE hInst);

	void SetFullRowSelect(bool enable);

//添加列标签
	void TListView::AddColumn(TCHAR text[], int width, int styleLVCFMT = LVCFMT_LEFT);

//插入列标签
	void TListView::InsertColumn(int index, TCHAR text[], int width, int styleLVCFMT = LVCFMT_LEFT);

//插入项目
	void AddItem(std::vector<std::string> items);

	void UpdateItem(int index, std::vector<std::string> items);

	void SetItem(int index, int col,std::tstring text);

	std::tstring GetItem(int index, int col);

	std::vector<int> GetCurSel();

	int TListView::GetColumnCount();

	int TListView::GetItemCount();//得到项目数

	void TListView::DeleteAllColumns();//清除全部项
	void TListView::DeleteAllItems();//清除全部项

	RECT TListView::GetGridRect(int index, int subitem);//根据行号和列号得到格子坐标

	RECT TListView::GetGridRectInMargin(int index, int subitem);//得到格子边框内的坐标
};

