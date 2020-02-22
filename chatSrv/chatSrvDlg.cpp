﻿
// chatSrvDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "chatSrv.h"
#include "chatSrvDlg.h"
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
private:
	CSockSrv m_sockSrv;
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


// CchatSrvDlg 对话框



CchatSrvDlg::CchatSrvDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSRV_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchatSrvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_LIST, m_msgListBox);
}

BEGIN_MESSAGE_MAP(CchatSrvDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BTN, &CchatSrvDlg::OnBnClickedStartBtn)
	ON_BN_CLICKED(IDC_SEND_BTN, &CchatSrvDlg::OnBnClickedSendBtn)
END_MESSAGE_MAP()


// CchatSrvDlg 消息处理程序

BOOL CchatSrvDlg::OnInitDialog()
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
	GetDlgItem(IDC_PORT_EDIT)->SetWindowText(_T("5000"));


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CchatSrvDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CchatSrvDlg::OnPaint()
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
HCURSOR CchatSrvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchatSrvDlg::OnBnClickedStartBtn()
{
	CString strIP, strPort;
	GetDlgItem(IDC_PORT_EDIT)->GetWindowText(strPort);
	USES_CONVERSION;
	LPCSTR cpPort = T2A(strPort);
	TRACE("[chatSrv]port:%s", cpPort);
	int iPort = _ttoi(strPort);

	//CSockSrv建立服务,在CSockSrv的accept内创建CSockChat
	m_sockSrv = new CSockSrv;
	if (!m_sockSrv->Create(iPort))
	{
		TRACE("m_sockSrv create error:%d", GetLastError());
		return;
	}
	
	if (!m_sockSrv->Listen())
	{
		TRACE("m_sockSrv listen error:%d", GetLastError());
		return;
	}
	CTime m_time = CTime::GetCurrentTime();
	CString str_tm = m_time.Format("%X");
	str_tm += _T("建立服务");

	m_msgListBox.AddString(str_tm);

	UpdateData(FALSE);//??把控件上内容更新至数据??
}


void CchatSrvDlg::OnBnClickedSendBtn()
{
	// 取得编辑框发送内容
	CString strMsg;
	GetDlgItem(IDC_MSG_EDIT)->GetWindowTextW(strMsg);

	USES_CONVERSION;
	LPCSTR cpMsg = T2A(strMsg);
	m_sockChat->Send(cpMsg, SEND_MAX_BUF, 0);

	CTime m_time = CTime::GetCurrentTime();
	strMsg = m_time.Format("%X") + "服务端发送: " + strMsg;
	// 添加到listBox
	m_msgListBox.AddString(strMsg);
	// 清空编辑框内容
	GetDlgItem(IDC_MSG_EDIT)->SetWindowTextW(_T(""));

}
