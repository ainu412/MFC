
// drawBySingleItem.h: drawBySingleItem 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// CdrawBySingleItemApp:
// 有关此类的实现，请参阅 drawBySingleItem.cpp
//

class CdrawBySingleItemApp : public CWinAppEx
{
public:
	CdrawBySingleItemApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	int m_nWidth;
};

extern CdrawBySingleItemApp theApp;
