#include "DialogHighScore.h"

#include <string>
#include "Configuration.h"

using namespace std;

extern Configuration config;

void DialogHighScore::FillListView(TListView& listView,TStatic &staticHighScore,TStatic &staticWinNum, const std::vector<std::shared_ptr<Configuration::Record>>& record)
{
	//添加列表标签
	listView.AddColumn("时间", 150);
	listView.AddColumn("种子", 90);
	listView.AddColumn("评估难度", 60);
	listView.AddColumn("最高分", 60);
	listView.AddColumn("是否解决", 60);

	int highScore = 0;
	unsigned int highScoreSeed = 0;
	int winNum = 0;
	int loseNum = 0;
	time_t t;
	for (auto& rec : record)
	{
		//列表框添加条目
		listView.AddItem(rec->ToVecString());

		//更新最高分
		if (rec->highScore > highScore)
		{
			highScore = rec->highScore;
			highScoreSeed = rec->seed;
			t = rec->time;
		}

		//计算胜负
		if (rec->solved)
			winNum++;
		else
			loseNum++;
	}

	staticHighScore.SetText("最高纪录：  " + to_string(highScore) + "  (时间="+time_tToString(t)+",种子=" + to_string(highScoreSeed) + ")");
	staticWinNum.SetText("胜：  " + to_string(winNum) + "    负：" + to_string(loseNum) + "    胜率：" + to_string(double(winNum) / (winNum + loseNum) * 100.0) + "%");
}

LRESULT DialogHighScore::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//设置最高纪录
	vecStaticHighScore.resize(3);
	vecStaticHighScore[0].LinkControl(m_hWnd, IDC_STATIC_HIGHSCORE);
	vecStaticHighScore[2] = vecStaticHighScore[1] = vecStaticHighScore[0];

	//设置获胜
	vecStaticWinNum.resize(3);
	vecStaticWinNum[0].LinkControl(m_hWnd, IDC_STATIC_WINNUM);
	vecStaticWinNum[2] = vecStaticWinNum[1] = vecStaticWinNum[0];

	//列表框
	vecListView.resize(3);
	vecListView[0].LinkControl(m_hWnd, IDC_LIST_RECORD);
	vecListView[0].SetFullRowSelect(true);

	vecListView[1] = vecListView[0];
	vecListView[2] = vecListView[0];

	//填充内容
	FillListView(vecListView[0],vecStaticHighScore[0],vecStaticWinNum[0], config.record1);
	FillListView(vecListView[1], vecStaticHighScore[1], vecStaticWinNum[1], config.record2);
	FillListView(vecListView[2], vecStaticHighScore[2], vecStaticWinNum[2], config.record4);

	//标签页
	tabControl.LinkControl(m_hWnd, IDC_TAB);
	tabControl.SetRectAsParent();
	tabControl.AddTabItem("初级", { &vecListView[0],&vecStaticHighScore[0],&vecStaticWinNum[0] });
	tabControl.AddTabItem("中级", { &vecListView[1],&vecStaticHighScore[1],&vecStaticWinNum[1] });
	tabControl.AddTabItem("高级", { &vecListView[2],&vecStaticHighScore[2],&vecStaticWinNum[2] });
	tabControl.SetCurSel(0);

	return 0;
}

LRESULT DialogHighScore::OnReplay(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//取得选中项
	int page = tabControl.GetCurSel();
	auto selection = vecListView[page].GetCurSel();
	if (selection.size() != 1)
	{
		MessageBox("只能选择一项。", "提示", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	//设置返回值
	ReturnType* pret = new ReturnType;
	pret->isRandom = false;
	auto GetSuit = [](int page)->int
	{
		switch (page)
		{
		case 0:return 1;
		case 1:return 2;
		case 2:return 4;
		}
	};
	pret->suit = GetSuit(page);
	pret->seed=stoul(vecListView[page].GetItem(selection.front(),1));
	pret->calc = 0;
	pret->solved = (vecListView[page].GetItem(selection.front(), 4)) == "已解决" ? true : false;

	//向父窗口发送新游戏消息，2=不弹窗
	::PostMessage(hParent, WM_COMMAND, MAKELONG(ID_NEW_GAME, 2), (LPARAM)pret);
	EndDialog(0);

	return 0;
}

LRESULT DialogHighScore::OnEvaluate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	return 0;
}