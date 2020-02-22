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
	// �������ݵ������
	CchatSrvDlg* dlg = (CchatSrvDlg*)AfxGetApp()->GetMainWnd();
	char recvBuf[SEND_MAX_BUF] = { 0 };//��ʱ�洢,�������ڴ�
	Receive(recvBuf, SEND_MAX_BUF, 0);

	TRACE("####Server szRecvBuf = %s", recvBuf);

	USES_CONVERSION;
	CString msg = A2W(recvBuf);

	// ��ʾ��������б��
	CTime m_time = CTime::GetCurrentTime();
	msg = m_time.Format("%X") + msg;

	dlg->m_msgListBox.AddString(msg);
	//dlg->UpdateData(FALSE);

	CAsyncSocket::OnReceive(nErrorCode);
}
