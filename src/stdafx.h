#pragma once
#define STRICT
#define VC_EXTRALEAN

#include <atlbase.h>        // 基本的ATL类
extern CComModule _Module;  // 全局_Module
#include <atlwin.h>         // ATL窗口类

#include <vector>
#include <queue>
#include <Windowsx.h>

#include "resource.h"

#pragma comment(lib,"comctl32.lib")