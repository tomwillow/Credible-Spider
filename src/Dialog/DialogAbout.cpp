#include "DialogAbout.h"

#include <string>

using namespace std;
LRESULT DialogAbout::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	edit.LinkControl(m_hWnd, IDC_EDIT);

	string s =  
"·自动翻牌使用回溯法+博弈树+估值函数+Hash重复局面判定实现。\r\n\r\n"
"·自动翻牌能解决90%以上的1花色牌局、20%左右的2花色牌局，4花色不能解出。\r\n\r\n"
"         献给我最爱的兔子。\r\n\r\n"
"         作者：Tom Willow\r\n"
"         tomwillow@qq.com\r\n\r\n"
"https://github.com/tomwillow/Credible-Spider";
	edit.SetText(s);

	return 0;
}