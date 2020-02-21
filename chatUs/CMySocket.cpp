#include "pch.h"
#include "CMySocket.h"

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}

void CMySocket::OnConnect(int nErrorCode)
{
	//TRACE("#####OnConnect");
	////在listBox上显示连接服务器成功
	//CchatUsDlg* dlg = (CchatUsDlg* )AfxGetApp()->GetMainWnd();
	//dlg->m_time = CTime::GetCurrentTime();
	////将时间转为string输出
	//CString strTime;
	//strTime = dlg->m_time.Format("%X");
	//dlg->m_listBox.AddString(strTime);

	//回调函数On需要在最后重写父类
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	TRACE("#####OnReceive");

}
