#include "pch.h"
#include "CSockChat.h"
#include "chatSrvDlg.h"

CSockChat::CSockChat()
{
}

CSockChat::~CSockChat()
{
}

void CSockChat::OnReceive(int nErrorCode)
{
	TRACE("####Server: OnReceive");
	// 接收数据到服务端
	CchatSrvDlg* dlg = (CchatSrvDlg*)AfxGetApp()->GetMainWnd();
	char recvBuf[SEND_MAX_BUF] = { 0 };//临时存储,不分配内存
	Receive(recvBuf, SEND_MAX_BUF, 0);
	TRACE("####Server szRecvBuf = %s", recvBuf);

	USES_CONVERSION;
	CString msg = A2W(recvBuf);

	// 显示到服务端列表框
	CTime m_time = CTime::GetCurrentTime();
	msg = m_time.Format("%X") + _T("服务端收到: ") + msg;

	dlg->m_msgListBox.AddString(msg);
	//dlg->UpdateData(FALSE);

	CAsyncSocket::OnReceive(nErrorCode);
}
