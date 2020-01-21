#include "DialogSearch.h"

#include "Manager.h"
#include "TStatic.h"
#include <string>
#include <thread>

using namespace std;

LRESULT DialogSearch::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	pSeed = make_shared<uint32_t>();
	pCalc = make_shared<int>();

	btnCancel.LinkControl(m_hWnd, IDCANCEL);

	SetWindowText("正在搜索可行解...");

	//取得CPU逻辑核数量
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	coreNum = sysInfo.dwNumberOfProcessors;
	coreNum =max(coreNum/2,1);

	int fontHeight = 18;
	int border = 10;
	int dHeight = (coreNum + 1) * fontHeight;
	RECT rectClient;
	GetClientRect(&rectClient);

	RECT rect;
	GetWindowRect(&rect);
	int captionHeight = (rect.bottom - rect.top) - rectClient.bottom;
	rect.bottom += dHeight;
	MoveWindow(&rect);

	RECT rectBtn;
	::GetWindowRect(btnCancel.GetHWND(), &rectBtn);
	int x = rectBtn.left - rect.left;
	int y = rectBtn.top - rect.top-captionHeight;
	int w = rectBtn.right - rectBtn.left;
	int h = rectBtn.bottom - rectBtn.top;
	y += dHeight+border;
	::MoveWindow(btnCancel.GetHWND(), x,y,w,h, 1);

	int width = rect.right - rect.left;
	staticMemo.Create(m_hWnd,{ border,border,width,fontHeight });
	staticMemo.SetText("线程数量：" + to_string(coreNum));
	staticMemo.SetDefaultGuiFont();

	for (int i = 0; i < coreNum; ++i)
	{
		vecStatic.emplace_back();
		vecStatic.back().Create(m_hWnd, { border,border+fontHeight * (i + 1),width,fontHeight });
		vecStatic.back().SetText("线程" + to_string(i));
		vecStatic.back().SetDefaultGuiFont();
	}

	CenterWindow();

	PostMessage(WM_COMMAND, MAKELONG(IDOK, 0));

	return TRUE;    // let the system set the focus
}

LRESULT DialogSearch::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (IsManagerOnThread() || searchThreadIsRunning)
	{
		StopManagerThread();
		stopThread = true;
		PostMessage(WM_CLOSE);
		return 0;

	}

	pSeed = nullptr;
	pCalc = nullptr;
	EndDialog(0);
	return 0;
}

bool DialogSearch::IsManagerOnThread()
{
	for (auto& manager : vecManager)
		if (manager && manager->bOnThread)
		{
			return true;
		}
	return false;
}

void DialogSearch::StopManagerThread()
{
	for (auto& manager : vecManager)
		if (manager->bOnThread)
		{
			manager->bStopThread = true;
		}
}

LRESULT DialogSearch::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int id = LOWORD(wParam);

	switch (id)
	{
	case IDOK:
	{
		//总线程
		auto fun = [&]()
		{
			searchThreadIsRunning = true;
			stopThread = false;

			//单个搜索线程
			auto search = [&](std::shared_ptr<Manager>& manager)
			{
				while (1)
				{
					//随机生成新牌局
					manager->Command("newrandom " + to_string(suit));
					if (manager->Command("auto"))
					{
						//完成搜索
						*pSeed = manager->GetPokerSeed();
						break;
					}

					//要求停止，则退出线程
					if (manager->bStopThread)
						break;
				}
			};

			//所有manager
			vecManager = vector<shared_ptr<Manager>>(coreNum, nullptr);

			//每个manager依次开启
			for (int i=0;i<coreNum;++i)
			{
				auto& manager = vecManager[i];
				manager = make_shared<Manager>();
				manager->SetTextOutputHWND(vecStatic[i].GetHWND());
				manager->bOnThread = true;

				thread t(search, manager);
				t.detach();
			}

			//判别
			shared_ptr<Manager> resultManager;
			while (IsManagerOnThread() || stopThread==false)
			{
				this_thread::sleep_for(10ms);
				bool found = false;

				//寻找是否有线程已完成
				for (auto& manager : vecManager)
					if (manager->autoSolveResult.success == true)
					{
						//得到结果
						resultManager = manager;
						found = true;
						break;
					}

				if (found)
					break;
			}

			//停止所有搜索线程
			StopManagerThread();

			if (resultManager)
			{
				stringstream ss;
				ss << "找到一组可行解：" << endl << endl;
				ss << "花色=" << resultManager->autoSolveResult.suit << endl;
				ss << "seed=" << resultManager->autoSolveResult.seed << endl;
				ss << "尝试次数=" << resultManager->autoSolveResult.calc << endl;
				ss << "难度=" << -10000.0 / resultManager->autoSolveResult.calc + 100.0 << endl;

				*pCalc = resultManager->autoSolveResult.calc;

				MessageBox(ss.str().c_str(), "求解结果", MB_OK | MB_ICONINFORMATION);
				searchThreadIsRunning = false;

				EndDialog(0);
			}
			searchThreadIsRunning = false;
		};

		thread t(fun);
		t.detach();
		break;
	}
	case IDCANCEL:

		SendMessage(WM_CLOSE);
		break;
	}

	return 0;
}