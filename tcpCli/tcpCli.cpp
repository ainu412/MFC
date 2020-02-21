#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	//加载并初始化套接字库
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsadata;
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)
	{
		return err;
	}
	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	//创建tcp套接字
	SOCKET sockCli = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	//addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//127.0.1为本机
	addrSrv.sin_port = htons(6000);

	//连接服务器
	connect(sockCli, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	//发送数据到服务器
	char sendBuf[100];
	sprintf_s(sendBuf, sizeof(sendBuf), "hello虾虾");
	send(sockCli, sendBuf, strlen(sendBuf) + 1, 0);

	//接收服务器发送的数据并打印
	char recvBuf[100];
	recv(sockCli, recvBuf, sizeof(recvBuf), 0);
	std::cout << recvBuf << std::endl;
	//关闭套接字并清理套接库
	closesocket(sockCli);
	WSACleanup();

	system("pause");
	return 0;
}

