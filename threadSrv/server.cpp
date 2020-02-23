//socket: tcp
//互斥体多线程同步

#include <iostream>
#include <WinSock2.h>
#include <process.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")

#define MAX_BACKLOG 5
#define MAX_BUF 1024
#define MAX_CONN 64
// 服务端的设计：
// 1 每来一个连接，服务端起一个线程（安排一个工人）维护
// 2 将收到的消息转发给所有的客户端
// 3 某个连接断开，需要处理断开的连接

// 全局变量:记得互锁
HANDLE hMutex;
SOCKET sockConns[MAX_CONN];
int connCnt = 0;

void sendMsg(char* msg, int nLen)
{
	WaitForSingleObject(hMutex, INFINITE);
	for (size_t i = 0; i < connCnt; i++)
	{
		send(sockConns[i], msg, nLen, 0);
	}
	ReleaseMutex(hMutex);
}

unsigned WINAPI hConn(void* arg)
{
	SOCKET sockConn = *(SOCKET*)arg;

	// 收到信息发给所有连接客户端
	char recvBuf[MAX_BUF] = { 0 };
	int nRecv;
	while (nRecv = recv(sockConn, recvBuf, MAX_BUF, 0) != 0)
	{
		sendMsg(recvBuf, nRecv);
	}

	// 将此连接断开
	WaitForSingleObject(hMutex, INFINITE);
	for (size_t i = 0; i < connCnt; i++)
	{
		if (sockConn == sockConns[i])
		{
			connCnt--;
			for (; i < connCnt; i++)
			{
				sockConns[i] = sockConns[i + 1];
			}
			closesocket(sockConn);
			std::cout << "断开后, 此时连接数目:" << connCnt << std::endl;
			break;
		}
	}
	ReleaseMutex(hMutex);
	return 0;
}

int main()
{
	// 加载套接字库
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	INT err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return err;
	}
	if (LOBYTE(wsaData.wVersion) != 1 
		|| HIBYTE(wsaData.wVersion) != 1) {
		return -1;
	}
	// 新建套接字并初始化
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	// 新建地址
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_port = htons(7000);
	addrSrv.sin_family = AF_INET;
	// 绑定
	int len = sizeof(SOCKADDR);
	bind(sockSrv, (SOCKADDR*)&addrSrv, len);
	// 监听
	listen(sockSrv, MAX_BACKLOG);
	// 新建客户套接字地址供接收
	SOCKADDR_IN addrCli;
	// 新建互斥体
	hMutex = CreateMutex(NULL, FALSE, NULL);

	// 接收并发送
	while (true)
	{		
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrCli, &len);

		// 加入连接套接字组
		WaitForSingleObject(hMutex, INFINITE);
		sockConns[connCnt++] = sockConn;
		ReleaseMutex(hMutex);
		// 开始套接字函数的线程,传入连接参数
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, hConn, &sockConn, 0, NULL);
#if 0
		char recvBuf[MAX_BUF];
		char sendBuf[MAX_BUF];// TODO 初始化
		recv(sockConn, recvBuf, MAX_BUF, 0);
		send(sockConn, sendBuf, MAX_BUF, 0);
		closesocket(sockConn);// 记得关闭!!
#endif
		closesocket(sockConn);
	}
	// 关闭并清理资源
	CloseHandle(hMutex);
	WSACleanup();
	closesocket(sockSrv);

	return 0;
}