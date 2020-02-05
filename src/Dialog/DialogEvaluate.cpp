#include "DialogEvaluate.h"

#include <string>
#include <thread>

#include <sstream>
#include <queue>

using namespace std;

LRESULT DialogEvaluate::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	btnCancel.LinkControl(m_hWnd, IDCANCEL);

	SetWindowText("正在评估...");

	//取得CPU逻辑核数量
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	threadNum = sysInfo.dwNumberOfProcessors;
	threadNum = min(threadNum / 2, input.size());

	int fontHeight = 18;//字高
	int border = 10;//间隔
	int dHeight = (threadNum + 1) * fontHeight;//总增加高度
	RECT rectClient;
	GetClientRect(&rectClient);

	RECT rect;
	GetWindowRect(&rect);
	int captionHeight = (rect.bottom - rect.top) - rectClient.bottom;
	rect.bottom += dHeight;//增加高度
	rect.right += 100;//增加宽度
	MoveWindow(&rect);

	//对话框总宽度
	int width = rect.right - rect.left;

	//设置按钮位置
	RECT rectBtn;
	::GetWindowRect(btnCancel.GetHWND(), &rectBtn);
	int x = rectBtn.left - rect.left;
	int y = rectBtn.top - rect.top - captionHeight;
	int w = rectBtn.right - rectBtn.left;
	int h = rectBtn.bottom - rectBtn.top;
	x = (width - w) / 2;//按钮居中
	y += dHeight + border;//按钮向下移动
	::MoveWindow(btnCancel.GetHWND(), x, y, w, h, 1);

	//线程数量文本
	staticMemo.Create(m_hWnd, { border,border,width,fontHeight });
	staticMemo.SetText("线程数量：" + to_string(threadNum));
	staticMemo.SetDefaultGuiFont();

	//设置线程文本
	for (int i = 0; i < threadNum; ++i)
	{
		vecStatic.emplace_back();
		vecStatic.back().Create(m_hWnd, { border,border + fontHeight * (i + 1),width,fontHeight });
		vecStatic.back().SetDefaultGuiFont();
	}

	CenterWindow();

	//初始化返回值
	ret = make_shared<unordered_map<uint32_t, ReturnType>>();

	for (auto& returnType : input)
	{
		(*ret).insert({ returnType.seed,returnType });
	}

	PostMessage(WM_COMMAND, MAKELONG(IDOK, 0));

	return 0;
}

LRESULT DialogEvaluate::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//通知manager停止
	for (auto& manager : managers)
		manager->bStopThread = true;

	if (onThread == false)
		EndDialog(0);

	return 0;
}

LRESULT DialogEvaluate::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int id = LOWORD(wParam);

	switch (id)
	{
	case IDOK:
	{
		auto mainThread = [&]()
		{
			onThread = true;

			//已处理数
			int solvedNum = 0;

			mutex m;

			//加入待处理队列
			queue<ReturnType> q;
			for (auto& returnType : input)
				q.push(returnType);

			//评估线程
			auto EvaluateThread = [&](int index)
			{
				while (1)
				{
					m.lock();

					//队列已空，退出
					if (q.empty())
					{
						m.unlock();
						break;
					}

					//取出一个待处理项
					auto raw = q.front();
					q.pop();

					//新建manager并加入managers
					shared_ptr<Manager> manager = make_shared<Manager>(suit, raw.seed);
					managers.insert(manager);

					m.unlock();

					//设置控件
					vecStatic[index].SetText("线程" + to_string(index) + " 种子=" + to_string(raw.seed));
					manager->SetTextOutputHWND(vecStatic[index].GetHWND());

					//开始评估
					if (manager->Command("auto"))
						solvedNum++;

					m.lock();

					//保存结果
					(*ret)[raw.seed].solved = manager->autoSolveResult.success;
					(*ret)[raw.seed].calc = manager->autoSolveResult.calc;

					m.unlock();

					//设置控件
					vecStatic[index].SetText("线程" + to_string(index) + " 已退出。");
				}
			};

			//创建线程
			vector<thread> vecThread;
			for (int index = 0; index < threadNum; ++index)
			{
				vecThread.push_back(thread(EvaluateThread, index));
			}

			//启动线程
			for_each(vecThread.begin(), vecThread.end(), [](auto& t) {t.join(); });

			//弹窗
			stringstream ss;
			ss << "评估已结束。" << endl << endl;
			ss << "完成评估：" << solvedNum << "/" << ret->size();

			MessageBox(ss.str().c_str(), "评估结束", MB_OK | MB_ICONINFORMATION);

			onThread = false;

			SendMessage(WM_CLOSE);
		};

		thread t(mainThread);
		t.detach();
		break;
	}
	case IDCANCEL:
	{
		SendMessage(WM_CLOSE);
		break;
	}
	}

	return 0;
}