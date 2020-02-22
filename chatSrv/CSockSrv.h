#pragma once
#include <afxsock.h>
#include "chatSrv.h"
#include "chatSrvDlg.h"

class CSockSrv :
	public CAsyncSocket
{
public:
	CSockSrv();
	virtual ~CSockSrv();
	virtual void OnAccept(int nErrorCode);
};

