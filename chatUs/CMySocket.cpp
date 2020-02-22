#include "pch.h"
#include "CMySocket.h"
#include "chatUs.h"
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
	CTime m_time = CTime::GetCurrentTime();
	CString str_time = m_time.Format("%X");
	//��ӳɹ���ʾCString����д��msgListBox
	str_time += _T("�ɹ������������");
	dlg->m_msgListBox.AddString(str_time);

	//�ǵò��丸��ص�����
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	// ������Ϣ
	TRACE("####OnReceive");
	char cpRecvBuf[SEND_MAX_BUF] = { 0 };
	Receive(cpRecvBuf, SEND_MAX_BUF, 0);
	//תΪCString
	USES_CONVERSION;
	CString strRecv = A2W(cpRecvBuf);
	
	CTime m_time = CTime::GetCurrentTime();
	strRecv = m_time.Format("%X") + strRecv;

	// ������Ի����listBox
	CchatUsDlg* dlg = (CchatUsDlg*)AfxGetApp()->GetMainWnd();

	dlg->m_msgListBox.AddString(strRecv);//���ַ������,����Ҫ��ǰ�����ʱ��,�Ա㰴˳������
	dlg->m_msgListBox.UpdateData(FALSE);

	//�ص����ຯ��
	CAsyncSocket::OnReceive(nErrorCode);

	// ʵ���Զ��ظ�
	// ����Զ��ظ�������
	CString strAuto;
	dlg->GetDlgItemText(IDC_AUTORESMSG_EDIT, strAuto);

	// �ж��Ƿ�ѡ�Զ��ظ�
	if (((CButton*)(dlg->GetDlgItem(IDC_AUTORESPONSE_RADIO)))->GetCheck()
		&& strAuto.GetLength() > 0)
	{
		// ���ʽ
		strAuto = dlg->m_name + _T("[�Զ��ظ�]: ") + strAuto;
		// �Զ��ظ�
		//USES_CONVERSION;
		LPWSTR cpAuto = (LPWSTR)T2A(strAuto);
		Send(cpAuto, SEND_MAX_BUF, 0);//Or dlg->m_sockCli->Send

		// ��ʾ������listBox
		dlg->m_msgListBox.AddString(m_time.Format("%X") + strAuto);
		dlg->m_msgListBox.UpdateData(FALSE);
	}
}
