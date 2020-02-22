#include "pch.h"
#include "CMySocket.h"
#include "chatUsDlg.h"

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}

void CMySocket::OnConnect(int nErrorCode)
{
	TRACE("####OnConnect");
	//获取点击客户端dlg的时间
	CchatUsDlg* dlg = (CchatUsDlg*)AfxGetApp()->GetMainWnd();
	//将取得的时间转化为字符串格式
	CString str_time = dlg->m_time.Format("%X");
	//添加成功提示CString后再写入msgListBox
	str_time += _T("成功与服务器连接");
	dlg->m_msgListBox.AddString(str_time);

	//记得补充父类回调函数
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
}
