#include "pch.h"
#include "CSockSrv.h"

CSockSrv::CSockSrv()
{
}

CSockSrv::~CSockSrv()
{
}
class CSockChat;
void CSockSrv::OnAccept(int nErrorCode)// TODO
{
	TRACE("####OnAccept");
	CchatSrvDlg* dlg = (CchatSrvDlg*)AfxGetApp()->GetMainWnd();
	dlg->m_sockChat = new CSockChat;
	dlg->m_time = CTime::GetCurrentTime();
	CString str_time = dlg->m_time.Format("%X");
	str_time += _T("客户端连接成功");

	dlg->m_msgListBox.AddString(str_time);
	dlg->m_msgListBox.UpdateData(FALSE);
	
	CAsyncSocket::OnAccept(nErrorCode);
}
