#include "pch.h"
#include "CSockSrv.h"

CSockSrv::CSockSrv()
{
}

CSockSrv::~CSockSrv()
{
}

void CSockSrv::OnAccept(int nErrorCode)
{
	TRACE("####OnAccept");
	CchatSrvDlg* dlg = (CchatSrvDlg*)AfxGetApp()->GetMainWnd();

}
