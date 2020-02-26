
// MFCDllDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCDll.h"
#include "MFCDllDlg.h"
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


// CMFCDllDlg 对话框



CMFCDllDlg::CMFCDllDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDLL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDllDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD_BTN, &CMFCDllDlg::OnBnClickedAddBtn)
	ON_BN_CLICKED(IDC_SUBSTRACT_BTN, &CMFCDllDlg::OnBnClickedSubstractBtn)
	ON_BN_CLICKED(IDC_PRINT_BTN, &CMFCDllDlg::OnBnClickedPrintBtn)
END_MESSAGE_MAP()


// CMFCDllDlg 消息处理程序

BOOL CMFCDllDlg::OnInitDialog()
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

void CMFCDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCDllDlg::OnPaint()
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
HCURSOR CMFCDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// extern int MyAdd(int a, int b);
// extern int MySubstract(int a, int b);
// _declspec(dllimport) int MyAdd(int a, int b);
// _declspec(dllimport) int MySubstract(int a, int b);

void CMFCDllDlg::OnBnClickedAddBtn()
{
	CString str;
	str.Format(L"3+2=%d", MyAdd(3, 2));
	MessageBox(str);
	
}


void CMFCDllDlg::OnBnClickedSubstractBtn()
{
	CString str;
	str.Format(L"3-2=%d", MySubstract(3, 2));
	MessageBox(str);
}


void CMFCDllDlg::OnBnClickedPrintBtn()
{
	ltPrint();
// 	pp.middlePrint();

// 	//1 动态加载dll
// 	HINSTANCE hInst = LoadLibraryA("MyDllDynamicLoading.dll");
// 	//2 声明要加载的函数  定义函数指针类型
// 	typedef int(_stdcall* RTPROC)();
// 	//3 从DLL获取函数的地址  A 通过导出函数的实际函数名 B通过序号访问DLL的导出函数
// 	RTPROC Add = (RTPROC)GetProcAddress(hInst, "rbPrint");// √
// 	//ADDPROC Add = (ADDPROC)GetProcAddress(hInst, "?Add@@YAHHH@Z");
// 	//ADDPROC Add = (ADDPROC)GetProcAddress(hInst, MAKEINTRESOURCEA(1));
// 
// 	if (!Add)
// 	{
// 		MessageBox(_T("获取函数地址失败"));
// 		TRACE("GetProcAddress errorcode = %d", GetLastError());
// 		return;
// 	}
// 	// 4 调用函数
// 	Add();

// 	//2 声明要加载的函数  定义函数指针类型
// 	typedef int(* RBPROC)();
// 	//3 从DLL获取函数的地址  A 通过导出函数的实际函数名 B通过序号访问DLL的导出函数
// 	//RBPROC Add2 = (RBPROC)GetProcAddress(hInst, "?rbPrint@@YAXXZ");
// 	RBPROC Add2 = (RBPROC)GetProcAddress(hInst, MAKEINTRESOURCEA(1));
// 
// 	if (!Add2)
// 	{
// 		MessageBox(_T("获取函数地址失败"));
// 		TRACE("GetProcAddress errorcode = %d", GetLastError());
// 		return;
// 	}
// 	// 4 调用函数
// 	Add2();

	rttPrint();//最常用: def中写,include中含,属性中填,其他都正常来

//	FreeLibrary(hInst);
}
