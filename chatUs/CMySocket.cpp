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
	////��listBox����ʾ���ӷ������ɹ�
	//CchatUsDlg* dlg = (CchatUsDlg* )AfxGetApp()->GetMainWnd();
	//dlg->m_time = CTime::GetCurrentTime();
	////��ʱ��תΪstring���
	//CString strTime;
	//strTime = dlg->m_time.Format("%X");
	//dlg->m_listBox.AddString(strTime);

	//�ص�����On��Ҫ�������д����
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	TRACE("#####OnReceive");

}
