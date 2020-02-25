
// ChildView.cpp: CChildView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "MailslotSend.h"
#include "ChildView.h"
#include <tchar.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_hAnoPipeRead = GetStdHandle(STD_INPUT_HANDLE);
	m_hAnoPipeWrite = GetStdHandle(STD_OUTPUT_HANDLE);
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
	ON_COMMAND(ID_SEND_SLOT, &CChildView::OnSendSlot)
	ON_COMMAND(ID_ANONYMOUS_PIPE_SEND, &CChildView::OnAnonymousPipeSend)
	ON_COMMAND(ID_ANONYMOUS_PIPE_RECV, &CChildView::OnAnonymousPipeRecv)
	ON_COMMAND(ID_NAMED_PIPE_SEND, &CChildView::OnNamedPipeSend)
	ON_COMMAND(ID_NAMED_PIPE_RECV, &CChildView::OnNamedPipeRecv)
	ON_COMMAND(ID_NAMED_PIPE_CONNECT, &CChildView::OnNamedPipeConnect)
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



void CChildView::OnSendSlot()
{
	// 创建文件句柄
	LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\MyMailslot");
	HANDLE hFile = 
		CreateFile(SlotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateFile failed with %d\n", GetLastError());
		return;
	}
	// 写入数据
	char sendBuf[] = "肉肉好!";
	DWORD written;
	if (!WriteFile(hFile, sendBuf, strlen(sendBuf) + 1, &written, NULL)) {
		MessageBox(L"写入数据失败!");
		CloseHandle(hFile);
		return;
	}
	CloseHandle(hFile);
}


void CChildView::OnAnonymousPipeSend()
{
	char sendBuf[128] = "客户端子进程发送";
	if (!WriteFile(m_hAnoPipeWrite, sendBuf, 128, NULL, NULL))
	{
		MessageBox(L"写入失败!");//L与_T()都行
		return;
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


void CChildView::OnNamedPipeSend()
{
	char sendBuf[1024] = "命名管道客户君说:肉肉最傻啦!";
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


void CChildView::OnNamedPipeConnect()
{
	// 等待命名管道
	LPCTSTR szName = TEXT("\\\\.\\pipe\\MyNamedPipe");
	if (!WaitNamedPipe(szName, INFINITE))
	{
		MessageBox(L"等待命名管道失败!");
		TRACE("等待命名管道失败!error code %d", GetLastError());
		CloseHandle(m_hNamedPipe);
		m_hNamedPipe = NULL;
		return;
	}
	
	// 新建文件接收h命名管道
	m_hNamedPipe = CreateFile(szName, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == m_hNamedPipe)////仅此写入与接收, 服务端是创建, 故阻止共享
	{
		MessageBox(L"文件创建失败!");
		TRACE("文件创建失败!error code %d", GetLastError());
		CloseHandle(m_hNamedPipe);
		m_hNamedPipe = NULL;
		return;
	}
}
