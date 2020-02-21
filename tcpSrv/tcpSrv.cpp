// tcpSrv.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	//加载套接字库
	WORD wVersionRequested;
	WSADATA wsadata;
	int err;
	//选择1.1版本
	wVersionRequested = MAKEWORD(1, 1);
	//初始化套接字库
	err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)
	{
		return err;
	}
	if (LOBYTE(wsadata.wVersion)!=1 || HIBYTE(wsadata.wVersion)!=1)
	{
		WSACleanup();
		return -1;
	}

	//新建服务器套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	//新建服务器地址
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");服务器绑定全部,客户端连本机
	addrSrv.sin_port = htons(6000);

	//绑定套接字至本地IP地址
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//长度同_IN

	//开始监听用户
	listen(sockSrv, 10);//backlog最多同时监听数

	std::cout << "Server start at port: 6000" << std::endl;

	SOCKADDR_IN addrCli;
	int len = sizeof(SOCKADDR);
	while (true)
	{
		//接收客户连接
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrCli, &len);
		char recvBuf[100];
		char sendBuf[100];
		sprintf_s(sendBuf, sizeof(recvBuf), "Hello肉肉");
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		recv(sockConn, recvBuf, sizeof(recvBuf), 0);
		//打印接收的数据
		std::cout << recvBuf << std::endl;
		closesocket(sockConn);
	}
	//关闭套接字库
	closesocket(sockSrv);
	//清理套接字库
	WSACleanup();
	return 0;
}
