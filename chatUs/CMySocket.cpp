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
	CTime m_time = CTime::GetCurrentTime();
	CString str_time = m_time.Format("%X");
	//添加成功提示CString后再写入msgListBox
	str_time += _T("成功与服务器连接");
	dlg->m_msgListBox.AddString(str_time);

	//记得补充父类回调函数
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	// 接收信息
	TRACE("####OnReceive");
	char cpRecvBuf[SEND_MAX_BUF] = { 0 };
	Receive(cpRecvBuf, SEND_MAX_BUF, 0);
	//转为CString
	USES_CONVERSION;
	CString strRecv = A2W(cpRecvBuf);

	CTime m_time = CTime::GetCurrentTime();
	strRecv = m_time.Format("%X") + "服务端发送: " + strRecv;

	// 输出到对话框的listBox
	CchatUsDlg* dlg = (CchatUsDlg*)AfxGetApp()->GetMainWnd();

	dlg->m_msgListBox.AddString(strRecv);//按字符排序的,故需要在前面添加时间,以便按顺序排列

	//回调父类函数
	CAsyncSocket::OnReceive(nErrorCode);
}
