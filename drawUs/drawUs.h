
// drawUs.h: drawUs 应用程序的主头文件
//
#pragma once

enum class DRAW_TYPE
{
	DT_PEN,
	DT_LINE,
	DT_ELLIPSE,
	DT_RECT
};
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// CdrawUsApp:
// 有关此类的实现，请参阅 drawUs.cpp
//

class CdrawUsApp : public CWinApp
{
public:
	CdrawUsApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CdrawUsApp theApp;
