
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
	hAnoPipeWrite = NULL;
	hAnoPipeRead = NULL;
}

CChildView::~CChildView()
{
	if (hAnoPipeRead)
	{
		CloseHandle(hAnoPipeRead);
		hAnoPipeRead = NULL;
	}
	if (hAnoPipeWrite)
	{
		CloseHandle(hAnoPipeWrite);
		hAnoPipeWrite = NULL;
	}
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_RECV_SLOT, &CChildView::OnRecvSlot)
	ON_COMMAND(ID_ANONYMOUS_PIPE_CREATE, &CChildView::OnAnonymousPipeCreate)
	ON_COMMAND(ID_ANONYMOUS_PIPE_SEND, &CChildView::OnAnonymousPipeSend)
	ON_COMMAND(ID_ANONYMOUS_PIPE_RECV, &CChildView::OnAnonymousPipeRecv)
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
	//HANDLE hAnoPipeRead,hAnoPipeWrite;发送接收函数中扔要使用,故设为成员变量
	SECURITY_ATTRIBUTES PipeAttributes;
	PipeAttributes.bInheritHandle = TRUE;
	PipeAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	PipeAttributes.lpSecurityDescriptor = NULL;
	if (!CreatePipe(&hAnoPipeRead, &hAnoPipeWrite, &PipeAttributes, 0))
	{
		MessageBox(L"邮槽创建失败!");
		//CloseHandle(hAnoPipeWrite);
		//CloseHandle(hAnoPipeRead);
		return;
	}
	//创建子进程
	STARTUPINFO StartupInfo; //用来指定新进程窗口如何显示
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);//记得取得标准把手!
	StartupInfo.hStdInput = hAnoPipeRead;
	StartupInfo.hStdOutput = hAnoPipeWrite;// 输出是写!!
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
		CloseHandle(hAnoPipeRead);
		CloseHandle(hAnoPipeWrite);
		hAnoPipeWrite = NULL;
		hAnoPipeRead = NULL;
	}
}


void CChildView::OnAnonymousPipeSend()
{
	char sendBuf[128] = "服务端发送";
	if (!WriteFile(hAnoPipeWrite, sendBuf, 128, NULL, NULL)) {
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
	if (!ReadFile(hAnoPipeRead, recvBuf, 128, NULL, NULL))
	{
		MessageBox(L"读取失败!");
		return;
	} 
	else
	{
		MessageBox((CStringW)recvBuf);
	}
}
