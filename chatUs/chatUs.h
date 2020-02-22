
// chatUs.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#define SEND_MAX_BUF 1024
#define NAME_MAX_BUF 32

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CchatUsApp:
// 有关此类的实现，请参阅 chatUs.cpp
//

class CchatUsApp : public CWinApp
{
public:
	CchatUsApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CchatUsApp theApp;
