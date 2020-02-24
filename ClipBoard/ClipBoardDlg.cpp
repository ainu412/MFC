﻿
// ClipBoardDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ClipBoard.h"
#include "ClipBoardDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClipBoardDlg 对话框



CClipBoardDlg::CClipBoardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIPBOARD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClipBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClipBoardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BTN, &CClipBoardDlg::OnBnClickedSendBtn)
	ON_BN_CLICKED(IDC_RECV_BTN, &CClipBoardDlg::OnBnClickedRecvBtn)
END_MESSAGE_MAP()


// CClipBoardDlg 消息处理程序

BOOL CClipBoardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClipBoardDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClipBoardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClipBoardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClipBoardDlg::OnBnClickedSendBtn()
{
	// 1 打开剪贴板
	if (OpenClipboard() == 0)
	{
		printf("open clipboard error, error code=%d", GetLastError());
	}
	// 2 清空剪贴板
	EmptyClipboard();
	// 3 获得编辑框内容
	CStringW strSendW;
	CStringA strSendA;
	GetDlgItemText(IDC_SEND_EDIT, strSendW);
	strSendA = (CStringA)strSendW;
	//		分配内存对象, 句柄给handle
	//		global alloc后必须加锁globallLock
	HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, strSendA.GetLength() + 1);//为了获得句柄分配全局变量
	char* szSendBuf = (char*)GlobalLock(hMem);//分配锁住的内存空间
// 	USES_CONVERSION;
// 	char* szSend = T2A(strSend);
	strcpy_s(szSendBuf, strSendA.GetLength() + 1, strSendA);
	GlobalUnlock(hMem);
	// 4 将数据放入剪贴板
	SetClipboardData(CF_TEXT, hMem);
}


void CClipBoardDlg::OnBnClickedRecvBtn()
{
	// 打开剪贴板
	if (OpenClipboard() == 0)
	{
		printf("open clipboard error, error code=%d", GetLastError());
	}
	// 
	if (IsClipboardFormatAvailable(CF_TEXT))
	{
		// 向剪贴板索要数据
		HANDLE hBoard;
		hBoard = GetClipboardData(CF_TEXT);
		char* recvBuf;
		recvBuf = (char*)GlobalLock(hBoard);		
		USES_CONVERSION;
		LPCTSTR strBuf = A2W(recvBuf);
		GlobalUnlock(hBoard);//最好写在句柄赋值后的后面
		SetDlgItemText(IDC_RECV_EDIT, strBuf);
	}
	CloseClipboard();
}
