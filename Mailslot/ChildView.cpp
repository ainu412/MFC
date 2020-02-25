
// ChildView.cpp: CChildView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Mailslot.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_hAnoPipeWrite = NULL;
	m_hAnoPipeRead = NULL;
	m_hNamedPipe = NULL;
}

CChildView::~CChildView()
{
	if (m_hAnoPipeRead)
	{
		CloseHandle(m_hAnoPipeRead);
		m_hAnoPipeRead = NULL;
	}
	if (m_hAnoPipeWrite)
	{
		CloseHandle(m_hAnoPipeWrite);
		m_hAnoPipeWrite = NULL;
	}
	if (m_hNamedPipe)
	{
		CloseHandle(m_hNamedPipe);
		m_hNamedPipe = NULL;
	}
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_RECV_SLOT, &CChildView::OnRecvSlot)
	ON_COMMAND(ID_ANONYMOUS_PIPE_CREATE, &CChildView::OnAnonymousPipeCreate)
	ON_COMMAND(ID_ANONYMOUS_PIPE_SEND, &CChildView::OnAnonymousPipeSend)
	ON_COMMAND(ID_ANONYMOUS_PIPE_RECV, &CChildView::OnAnonymousPipeRecv)
	ON_COMMAND(ID_NAMED_PIPE_SEND, &CChildView::OnNamedPipeSend)
	ON_COMMAND(ID_NAMED_PIPE_RECV, &CChildView::OnNamedPipeRecv)
	ON_COMMAND(ID_NAMED_PIPE_CREATE, &CChildView::OnNamedPipeCreate)
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}



void CChildView::OnRecvSlot()
{
	// 创建邮槽
	LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\MyMailslot");
	HANDLE hSlot = CreateMailslot(SlotName, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (hSlot == INVALID_HANDLE_VALUE)
	{
		TRACE("Create mail slot failed with%d", GetLastError());
		return;
	}
	// 从文件中读取信息
	char recvBuf[128]={ 0 };//记得初始化哇!
	DWORD dwRead;
	if (!ReadFile(hSlot, recvBuf, 128, &dwRead, NULL))
	{
		MessageBox(L"读取数据失败!");
		CloseHandle(hSlot);
		return;
	}
	MessageBox(CStringW(recvBuf));
	CloseHandle(hSlot);
}


void CChildView::OnAnonymousPipeCreate()
{	
	// 新建匿名邮槽
	//HANDLE m_hAnoPipeRead,m_hAnoPipeWrite;发送接收函数中扔要使用,故设为成员变量
	SECURITY_ATTRIBUTES PipeAttributes;
	PipeAttributes.bInheritHandle = TRUE;
	PipeAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	PipeAttributes.lpSecurityDescriptor = NULL;
	if (!CreatePipe(&m_hAnoPipeRead, &m_hAnoPipeWrite, &PipeAttributes, 0))
	{
		MessageBox(L"邮槽创建失败!");
		//CloseHandle(m_hAnoPipeWrite);
		//CloseHandle(m_hAnoPipeRead);
		return;
	}
	//创建子进程
	STARTUPINFO StartupInfo; //用来指定新进程窗口如何显示
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);//记得取得标准把手!
	StartupInfo.hStdInput = m_hAnoPipeRead;
	StartupInfo.hStdOutput = m_hAnoPipeWrite;// 输出是写!!
	StartupInfo.dwFlags = STARTF_USESTDHANDLES;
	PROCESS_INFORMATION ProcessInformation;
	memset(&ProcessInformation, 0, sizeof(PROCESS_INFORMATION));
	int iRet = CreateProcess(_T("MailSlotSend.exe"), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &StartupInfo,
		&ProcessInformation);
	if (iRet)
	{
		CloseHandle(ProcessInformation.hThread);
		CloseHandle(ProcessInformation.hProcess);
		ProcessInformation.dwProcessId = 0;
		ProcessInformation.dwThreadId = 0;
		ProcessInformation.hThread = NULL;
		ProcessInformation.hProcess = NULL;
	}
	else
	{
		MessageBox(L"子进程创建失败!");
		CloseHandle(m_hAnoPipeRead);
		CloseHandle(m_hAnoPipeWrite);
		m_hAnoPipeWrite = NULL;
		m_hAnoPipeRead = NULL;
	}
}


void CChildView::OnAnonymousPipeSend()
{
	char sendBuf[128] = "服务端发送";
	if (!WriteFile(m_hAnoPipeWrite, sendBuf, 128, NULL, NULL)) {
		MessageBox(L"写入失败!");
		return;
	}
	else
	{
		MessageBox(L"发送成功!");
	}
}


void CChildView::OnAnonymousPipeRecv()
{
	char recvBuf[128] = { 0 };
	if (!ReadFile(m_hAnoPipeRead, recvBuf, 128, NULL, NULL))
	{
		MessageBox(L"读取失败!");
		return;
	} 
	else
	{
		MessageBox((CStringW)recvBuf);
	}
}

void CChildView::OnNamedPipeCreate()
{
	// 新建命名管道
	LPCTSTR szName = TEXT("\\\\.\\pipe\\MyNamedPipe");
	m_hNamedPipe = CreateNamedPipe(szName, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE, 1, 1024, 1024, 0, NULL);
	if (INVALID_HANDLE_VALUE == m_hNamedPipe)
	{
		MessageBox(L"命名管道创建失败!");
		TRACE("命名管道创建失败!error code %d", GetLastError());
		return;
	}
	// 等待连接命名管道
	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//每一个create后都要判断
	if (0 == hEvent)
	{
		MessageBox(L"事件创建失败!");
		TRACE("事件创建失败!error code %d", GetLastError());
		CloseHandle(m_hNamedPipe);
		m_hNamedPipe = NULL;
		return;
	}
	OVERLAPPED Overlapped;
	ZeroMemory(&Overlapped, sizeof(OVERLAPPED));//记得初始化为0哇!!
	Overlapped.hEvent = hEvent;
	if (0 == ConnectNamedPipe(m_hNamedPipe, &Overlapped))
	{
		if (ERROR_IO_PENDING != GetLastError())//用户悬而未决时可以继续等待的啦
		{
			MessageBox(L"命名管道开始等待连接失败(重叠I/0操作正在进行中)!");
			CloseHandle(m_hNamedPipe);
			m_hNamedPipe = NULL;
			CloseHandle(hEvent);
			hEvent = NULL;
			return;
		}
	}
	if (WAIT_FAILED == WaitForSingleObject(hEvent, INFINITE))
	{
		MessageBox(L"命名管道等待对象创建失败!");
		CloseHandle(m_hNamedPipe);
		m_hNamedPipe = NULL;
		CloseHandle(hEvent);
		hEvent = NULL;
		return;
	}
}

void CChildView::OnNamedPipeSend()
{
	char sendBuf[1024] = "命名管道服务君说:肉肉最可爱啦!";
	if (!WriteFile(m_hNamedPipe, sendBuf, 1024, NULL, NULL))
	{
		MessageBox(L"写入失败!");
		CloseHandle(m_hNamedPipe);
		m_hNamedPipe = NULL;
		return;
	}
}


void CChildView::OnNamedPipeRecv()
{
	char recvBuf[1024] = { 0 };
	if (!ReadFile(m_hNamedPipe, recvBuf, 1024, NULL, NULL))
	{
		MessageBox(L"读取失败!");
		CloseHandle(m_hNamedPipe);
		m_hNamedPipe = NULL;
		return;
	}
	MessageBox((CStringW)recvBuf);
}
