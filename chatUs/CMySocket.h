#pragma once
#include <afxsock.h>
#include "chatUs.h"
#include "chatUsDlg.h"

class CMySocket :
	public CAsyncSocket
{
public:
	CMySocket();
	virtual ~CMySocket();
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
public:
	CTime m_time;
};

