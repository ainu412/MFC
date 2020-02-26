
// MyRegeditDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyRegedit.h"
#include "MyRegeditDlg.h"
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


// CMyRegeditDlg 对话框



CMyRegeditDlg::CMyRegeditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYREGEDIT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyRegeditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyRegeditDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyRegeditDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyRegeditDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMyRegeditDlg 消息处理程序

BOOL CMyRegeditDlg::OnInitDialog()
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

void CMyRegeditDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyRegeditDlg::OnPaint()
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
HCURSOR CMyRegeditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyRegeditDlg::OnBnClickedButton1()
{
	HKEY hKey;
	DWORD dwAge = 22;
	//创建注册表项  VS2019自带的调试器管理员权限运行  自己的生成是以用户的权限运行
	int ret = ::RegCreateKeyW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\JESS\\admin", &hKey);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("##RegCreateKeyW Failed ,ErrorCode = %d,ret = %d", GetLastError(), ret);
		MessageBox(L"创建注册表失败");
		return;
	}
	//写注册表

	ret = ::RegSetValueEx(hKey, L"age", 0, REG_DWORD, (CONST BYTE*) & dwAge, 4);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("##RegSetValueEx Failed ,ErrorCode = %d,ret = %d", GetLastError(), ret);
		MessageBox(L"写注册表失败");
		return;
	}
	::RegCloseKey(hKey);
}


void CMyRegeditDlg::OnBnClickedButton2()
{
	HKEY hKey;
	DWORD dwAge;
	//创建注册表项  VS2019自带的调试器管理员权限运行  自己的生成是以用户的权限运行
	int ret = ::RegOpenKeyW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Rock\\admin", &hKey);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("##RegOpenKeyW Failed ,ErrorCode = %d,ret = %d", GetLastError(), ret);
		MessageBox(L"打开注册表失败");
		return;
	}

	//读注册表
	DWORD dwType;
	DWORD dwValue;
	ret = ::RegQueryValueEx(hKey, L"age", 0, &dwType, (LPBYTE)&dwAge, &dwValue);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("##RegQueryValueEx Failed ,ErrorCode = %d,ret = %d", GetLastError(), ret);
		MessageBox(L"读注册表失败");
		return;
	}
	TRACE("###dwType = %d,dwValue = %d ,dwAge = %d", dwType, dwValue, dwAge);
	::RegCloseKey(hKey);
}
