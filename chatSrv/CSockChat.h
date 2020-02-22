#pragma once
#include <afxsock.h>
#include <atlconv.h>

class CSockChat :
	public CAsyncSocket
{
public:
	CSockChat();
	virtual ~CSockChat();
	virtual void OnReceive(int nErrorCode);
};

