#pragma once
#include <afxsock.h>
class CSockChat :
	public CAsyncSocket
{
public:
	CSockChat();
	~CSockChat();
	virtual void OnReceive();
};

