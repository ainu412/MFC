
// chatUsDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "chatUs.h"
#include "chatUsDlg.h"
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


// CchatUsDlg 对话框



CchatUsDlg::CchatUsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATUS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchatUsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHATMSG_LIST, m_msgListBox);
	DDX_Control(pDX, IDC_FONTCOLOR_COMBO, m_fontColorCombo);
}

BEGIN_MESSAGE_MAP(CchatUsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT_BTN, &CchatUsDlg::OnBnClickedConnectBtn)
	ON_BN_CLICKED(IDC_DISCONNECT_BTN, &CchatUsDlg::OnBnClickedDisconnectBtn)
	ON_BN_CLICKED(IDC_SEND_BTN, &CchatUsDlg::OnBnClickedSendBtn)
	ON_BN_CLICKED(IDC_NAME_BTN, &CchatUsDlg::OnBnClickedNameBtn)
	ON_BN_CLICKED(IDC_AUTORESPONSE_RADIO, &CchatUsDlg::OnBnClickedAutoresponseRadio)
	ON_BN_CLICKED(IDC_CLS_BTN, &CchatUsDlg::OnBnClickedClsBtn)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CchatUsDlg 消息处理程序

BOOL CchatUsDlg::OnInitDialog()
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
	GetDlgItem(IDC_IPADDRESS)->SetWindowText(_T("192.168.43.158"));
	GetDlgItem(IDC_PORT_EDIT)->SetWindowText(_T("5000"));

	//显示当前昵称,每次打开时从配置文件中读取
	//LPWSTR cpCurPath;
	WCHAR cpCurPath[MAX_PATH] = { 0 };
	CString strFilePath;
	GetCurrentDirectory(MAX_PATH, cpCurPath);
	strFilePath.Format(L"%ls//ClientNickname.ini", cpCurPath);

	TRACE(L"file open path:%s", strFilePath);

	WCHAR cpName[NAME_MAX_BUF] = { 0 };
	int i = GetPrivateProfileStringW(L"CLIENT", L"NICKNAME", NULL, cpName, NAME_MAX_BUF, strFilePath);

	if (!i)
	{
		SetDlgItemText(IDC_NICKNAME_EDIT, L"未设置");
		m_name = "客户端";
	}
	else
	{
		// 显示至控件
		SetDlgItemText(IDC_NICKNAME_EDIT, cpName);
		m_name = cpName;
		TRACE(L"file read name:%s", cpName);
	}

	// 设置按钮初态是否可用
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONNECT_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_DISCONNECT_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTORESPONSE_RADIO)->EnableWindow(FALSE);

	// 添加字体颜色
	m_fontColorCombo.AddString(_T("黑色"));
	m_fontColorCombo.AddString(_T("蓝色"));
	m_fontColorCombo.AddString(_T("绿色"));
	m_fontColorCombo.AddString(_T("红色"));
	m_fontColorCombo.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CchatUsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CchatUsDlg::OnPaint()
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
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);

		CDC dcBmp;
		dcBmp.CreateCompatibleDC(&dcBmp);

		CBitmap cBmp;
		cBmp.LoadBitmap(IDB_BITMAP1);

		BITMAP Bmp;
		cBmp.GetBitmap(&Bmp);

		CBitmap* pOldBmp = dcBmp.SelectObject(&cBmp);

		dc.StretchBlt(0, 0, rect.Width(), rect.Width(), &dcBmp, 
			0, 0, Bmp.bmWidth, Bmp.bmHeight, SRCCOPY);

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CchatUsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchatUsDlg::OnBnClickedConnectBtn()
{
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_CONNECT_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DISCONNECT_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_AUTORESPONSE_RADIO)->EnableWindow(TRUE);

	TRACE("###OnBnClickedConnectBtn");//常用调试手段
	/*TRACE("[chatUs]Connect Btn");
	MessageBoxW(L"喜欢肉肉");*/
	//取得控件显示端口和IP
	CString strIP, strPort;
	GetDlgItem(IDC_IPADDRESS)->GetWindowText(strIP);
	GetDlgItem(IDC_PORT_EDIT)->GetWindowText(strPort);
	USES_CONVERSION;
	LPCSTR cpIP = (LPCSTR)T2A(strIP);
	LPCSTR cpPort = (LPCSTR)T2A(strPort);
	TRACE("[chatUs]IP:%s, port:%s", cpIP, cpPort);

	//创建socket对象
	m_sockCli = new CMySocket;
	//创建套接字
	if (!m_sockCli->Create())
	{
		TRACE("m_sockCli create error: %d", GetLastError());
	}
	//连接
	int iPort = _ttoi(strPort);
	if (m_sockCli->Connect(strIP, iPort) == SOCKET_ERROR)
	{
		TRACE("m_sockCli connect error: %d", GetLastError());
	}

}


void CchatUsDlg::OnBnClickedDisconnectBtn()
{
	// 1.切换按钮显示
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONNECT_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_DISCONNECT_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTORESPONSE_RADIO)->EnableWindow(FALSE);

	// 2.发送信息给服务端
	USES_CONVERSION;
	char* cpMsg = T2A(L"用户[" + m_name + _T("]已断开"));
	m_sockCli->Send(cpMsg, SEND_MAX_BUF, 0);
	// 3.清理资源
	m_sockCli->Close();
	if (m_sockCli!=NULL)
	{
		delete m_sockCli;
		m_sockCli = NULL;
	}
	// 4.在客户端屏幕显示断开
	m_msgListBox.AddString(_T("已断开!"));
}


void CchatUsDlg::OnBnClickedSendBtn()
{
	CString strShow;
	// 1.获取编辑框内容
	//GetDlgItem(IDC_RESMSG_EDIT)->GetWindowTextW(strShow);
	GetDlgItemText(IDC_RESMSG_EDIT, strShow);

	USES_CONVERSION;
	char* cpMsg = T2A(m_name + _T(": ") + strShow);

	// 2.发送给服务端

	m_sockCli->Send(cpMsg, SEND_MAX_BUF, 0);

	TRACE("[chatUsDlg]OnBnClickedSendBtn() send characters: %s", cpMsg);

	// 3.显示到m_list框
	CTime m_time = CTime::GetCurrentTime();
	strShow = m_time.Format("%X") + m_name + _T(": ") + strShow;
	m_msgListBox.AddString(strShow);

	UpdateData(FALSE);

	//清空m_list框
	//GetDlgItem(IDC_RESMSG_EDIT)->SetWindowTextW(_T(""));
	SetDlgItemText(IDC_RESMSG_EDIT, L"");
}


void CchatUsDlg::OnBnClickedNameBtn()
{
	// 获取填写名称
	CString strName;
	GetDlgItemText(IDC_NICKNAME_EDIT, strName);
	
	if (strName.GetLength() > 0 && strName.GetLength() < NAME_MAX_BUF 
		&& IDOK==AfxMessageBox(_T("确定修改昵称吗?"), MB_OKCANCEL) )//MB_OKCANCEL
	{
		// 获得当前目录所在路径
		//LPWSTR cpCurPath;
		WCHAR cpCurPath[MAX_PATH] = { 0 };
		GetCurrentDirectory(MAX_PATH, cpCurPath);		

		TRACE(L"current directory:%s", cpCurPath);

		// 将配置文件命名并存入当前目录
		CString strFilePath; 
		strFilePath.Format(L"%ls//ClientNickname.ini", cpCurPath);

		TRACE(L"file path:%s", strFilePath);

		// 写入配置文件
		WritePrivateProfileStringW(L"CLIENT", L"NICKNAME", strName, strFilePath);

		m_name = strName;
	}
	else if (strName.GetLength() <= 0)
	{
		MessageBoxW(L"昵称不能为空!");
	}
}


void CchatUsDlg::OnBnClickedAutoresponseRadio()
{
	if (((CButton*)GetDlgItem(IDC_AUTORESPONSE_RADIO))->GetCheck())
	{
		((CButton*)GetDlgItem(IDC_AUTORESPONSE_RADIO))->SetCheck(FALSE);
	} 
	else
	{
		((CButton*)GetDlgItem(IDC_AUTORESPONSE_RADIO))->SetCheck(TRUE);
	}
}


void CchatUsDlg::OnBnClickedClsBtn()
{
	m_msgListBox.ResetContent();
}


HBRUSH CchatUsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	CString strCol;
	m_fontColorCombo.GetWindowTextW(strCol);
	if (IDC_RESMSG_EDIT==pWnd->GetDlgCtrlID() 
		|| IDC_CHATMSG_LIST==pWnd->GetDlgCtrlID())
	{
		if (L"黑色" == strCol)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
		}
		else if (L"红色" == strCol)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else if (L"绿色" == strCol)
		{
			pDC->SetTextColor(RGB(0, 255, 0));
		}
		else if (L"蓝色" == strCol)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CchatUsDlg::PreTranslateMessage(MSG* pMsg)
{
	// 规避回车键
	//if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	//{
	//	TRACE("回车");
	//	return TRUE;
	//}
	// 规避空格键
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
	{
		TRACE("回车");
		return TRUE;
	}
	// 组合键ctrl+X退出
	if (pMsg->message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0)
	{
		TRACE("按下Ctrl键");
		if (pMsg->wParam == 'X')
		{
			TRACE("按下Ctrl+X");
			// 关闭对话框
			CDialogEx::OnOK();
		}
	}
	// 回车发送消息
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnBnClickedSendBtn();
		return TRUE;//规避回车关闭对话框
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
