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
	//��ȡ����ͻ���dlg��ʱ��
	CchatUsDlg* dlg = (CchatUsDlg*)AfxGetApp()->GetMainWnd();
	//��ȡ�õ�ʱ��ת��Ϊ�ַ�����ʽ
	CString str_time = dlg->m_time.Format("%X");
	//��ӳɹ���ʾCString����д��msgListBox
	str_time += _T("�ɹ������������");
	dlg->m_msgListBox.AddString(str_time);

	//�ǵò��丸��ص�����
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	TRACE("####OnReceive");

}
