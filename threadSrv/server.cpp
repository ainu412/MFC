//socket: tcp
//互斥体多线程同步

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

#define MAX_BACKLOG 5
#define MAX_BUF 1024
// 服务端的设计：
// 1 每来一个连接，服务端起一个线程（安排一个工人）维护
// 2 将收到的消息转发给所有的客户端
// 3 某个连接断开，需要处理断开的连接

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
	// 接收并发送
	while (true)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrCli, &len);
		char recvBuf[MAX_BUF];
		char sendBuf[MAX_BUF];// TODO 初始化
		recv(sockConn, recvBuf, MAX_BUF, 0);
		send(sockConn, sendBuf, MAX_BUF, 0);
		closesocket(sockConn);// 记得关闭!!
	}
	// 关闭并清理资源
	WSACleanup();
	closesocket(sockSrv);

	return 0;
}