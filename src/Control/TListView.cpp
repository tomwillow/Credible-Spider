#pragma once
#include "TListView.h"

#include <assert.h>
#include <tchar.h>
#include <stdio.h>

using namespace std;

TListView::TListView()
{
}

void TListView::DeleteAllItems()
{
	ListView_DeleteAllItems(m_hWnd);
}

int TListView::GetColumnCount()
{
	HWND header=ListView_GetHeader(m_hWnd);
	return Header_GetItemCount(header);
}

int TListView::GetItemCount()
{
	return ListView_GetItemCount(m_hWnd);
}

void TListView::AddItem(std::vector<std::string> items)
{
	LVITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT;
	lvi.iItem = GetItemCount();

	for (int i = 0; i < items.size(); ++i)
	{
		int sz = items[i].length() + 1;
		TCHAR* temp = new TCHAR[sz];
		strcpy_s(temp, sz, items[i].c_str());
		lvi.pszText = temp;

		lvi.iSubItem = i;

		if (i == 0)
			::SendMessage(m_hWnd, LVM_INSERTITEM, 0, (LPARAM)&lvi);
		else
			::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)&lvi);

		delete[] temp;
	}
}

void TListView::UpdateItem(int index,std::vector<std::string> items)
{
	LVITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT;
	lvi.iItem = index;

	for (int i = 0; i < items.size(); ++i)
	{
		int sz = items[i].length() + 1;
		TCHAR* temp = new TCHAR[sz];
		strcpy_s(temp, sz, items[i].c_str());
		lvi.pszText = temp;

		lvi.iSubItem = i;

		::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)&lvi);

		delete[] temp;
	}
}

void TListView::SetItem(int index, int col, std::tstring text)
{
	LVITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT;
	lvi.iItem = index;

	int sz = text.length() + 1;
	TCHAR* temp = new TCHAR[sz];
	strcpy_s(temp, sz, text.c_str());
	lvi.pszText = temp;

	lvi.iSubItem = col;

	::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)&lvi);

	delete[] temp;
}

std::tstring TListView::GetItem(int index, int col)
{
	TCHAR temp[128];
	ListView_GetItemText(m_hWnd, index, col, temp, sizeof(temp));

	return temp;
}

vector<int> TListView::GetCurSel()
{
	vector<int> selection;
	int count = ListView_GetSelectedCount(m_hWnd);

	if (count == 0)
		return {};

	int start = -1;
	for (int i = 0; i < count; ++i)
	{
		start=ListView_GetNextItem(m_hWnd, start, LVNI_SELECTED);
		selection.push_back(start);
	}

	return selection;
}

TListView::TListView(const TListView& listView)
{
	*this = listView;
}

TListView& TListView::operator=(const TListView& listView)
{
	TControl::operator=(listView);

	DWORD style=ListView_GetExtendedListViewStyle(listView.m_hWnd);
	ListView_SetExtendedListViewStyle(m_hWnd, style);

	return *this;
}

void TListView::SetFullRowSelect(bool enable)
{
	DWORD styles = ListView_GetExtendedListViewStyle(m_hWnd);
	styles |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(m_hWnd, styles, styles);
}

void TListView::CreateListViewEx(HWND hParent, UINT id, HINSTANCE hInst)
{
	m_hInst = hInst;
	m_hWnd = ::CreateWindowEx(0,
		WC_LISTVIEW,
		TEXT("TListView"),
		WS_CHILD | WS_VISIBLE | LVS_ICON | LVS_REPORT | LVS_SINGLESEL,//|LVS_EDITLABELS|WS_BORDER
		0, 0, 0, 0, hParent, (HMENU)id, hInst, 0);

	//设置扩展风格：添加格子线以及整行选中
	DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_hWnd, styles, styles);

	RegisterProc();
}


void TListView::InsertColumn(int index, TCHAR text[], int width, int styleLVCFMT)
{
 	LVCOLUMN lvc = { 0 };
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_SUBITEM | LVCF_ORDER;
	lvc.cx = width;
	lvc.fmt = styleLVCFMT;
	lvc.pszText = text;
	lvc.iSubItem = index;
	lvc.iOrder = index;
	assert(SendMessage(m_hWnd, LVM_INSERTCOLUMN, index, (LPARAM)&lvc) != -1);
}

void TListView::AddColumn(TCHAR text[], int width, int styleLVCFMT)
{
	InsertColumn(GetColumnCount(), text, width, styleLVCFMT);
}

RECT TListView::GetGridRectInMargin(int index, int subitem)
{
	RECT rect;
	rect = GetGridRect(index, subitem);
	rect.left += 1;
	rect.top += 1;
	rect.right -= 1;
	rect.bottom -= 1;
	return rect;
}

RECT TListView::GetGridRect(int index, int subitem)
{
	RECT rect;
	ListView_GetSubItemRect(m_hWnd, index, subitem, LVIR_BOUNDS, &rect);
	//对于第一列，需要特殊处理
	if (subitem == 0)
	{
		int nWidth0 = ListView_GetColumnWidth(m_hWnd, 0);
		rect.right = rect.left + nWidth0;
	}
	return rect;
}