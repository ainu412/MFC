#include "pch.h"
#include "CSockSrv.h"

CSockSrv::CSockSrv()
{
}

CSockSrv::~CSockSrv()
{
}

void CSockSrv::OnAccept(int nErrorCode)// TODO
{
	TRACE("####OnAccept");
	CchatSrvDlg* dlg = (CchatSrvDlg*)AfxGetApp()->GetMainWnd();

	dlg->m_sockChat = new CSockChat;
	Accept(*(dlg->m_sockChat));//���为��������ӵ�socket��!!!

	CTime m_time = CTime::GetCurrentTime();
	CString str_time = m_time.Format("%X") + _T("�ͻ������ӳɹ�");

	dlg->m_msgListBox.AddString(str_time);
	dlg->m_msgListBox.UpdateData(FALSE);
	
	CAsyncSocket::OnAccept(nErrorCode);
}
