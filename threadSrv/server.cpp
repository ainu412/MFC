////socket: tcp
////互斥体多线程同步
//
//#include <iostream>
//#include <WinSock2.h>
//#include <process.h>
//#include <Windows.h>
//#pragma comment(lib,"ws2_32.lib")
//
//#define MAX_BACKLOG 5
//#define MAX_BUF 1024
//#define MAX_CONN 64
//#define PORT_NUM 9190
//// 服务端的设计：
//// 1 每来一个连接，服务端起一个线程（安排一个工人）维护
//// 2 将收到的消息转发给所有的客户端
//// 3 某个连接断开，需要处理断开的连接
//
//// 全局变量:记得互锁
//HANDLE hMutex;
//SOCKET sockConns[MAX_CONN];
//int connCnt = 0;
//
//void sendMsg(char* msg, int nLen)
//{
//	WaitForSingleObject(hMutex, INFINITE);
//	for (int i = 0; i < connCnt; i++)
//	{
//		send(sockConns[i], msg, nLen, 0);
//	}
//	ReleaseMutex(hMutex);
//}
//
//unsigned WINAPI hConn(void* arg)
//{
//	SOCKET sockConn = *(SOCKET*)arg;
//
//	// 收到信息发给所有连接客户端
//	char recvBuf[MAX_BUF] = { 0 };
//	int nRecv;
//	while (nRecv = recv(sockConn, recvBuf, MAX_BUF, 0) != 0)
//	{
//		sendMsg(recvBuf, nRecv);
//	}
//
//	// 将此连接断开
//	WaitForSingleObject(hMutex, INFINITE);
//	for (int i = 0; i < connCnt; i++)
//	{
//		if (sockConn == sockConns[i])
//		{
//			connCnt--;
//			for (; i < connCnt; i++)
//			{
//				sockConns[i] = sockConns[i + 1];
//			}
//			closesocket(sockConn);
//			std::cout << "断开后, 此时连接数目:" << connCnt << std::endl;
//			break;
//		}
//	}
//	ReleaseMutex(hMutex);
//	return 0;
//}
//
//int main()
//{
//	// 加载套接字库
//	WORD wVersionRequested = MAKEWORD(1, 1);
//	WSADATA wsaData;
//	INT err = WSAStartup(wVersionRequested, &wsaData);
//	if (err != 0)
//	{
//		return err;
//	}
//	if (LOBYTE(wsaData.wVersion) != 1
//		|| HIBYTE(wsaData.wVersion) != 1) {
//		WSACleanup();//记得传输错误后清理资源
//		return -1;
//	}
//	// 新建套接字并初始化
//	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
//	// 新建地址
//	SOCKADDR_IN addrSrv;
//	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//	addrSrv.sin_port = htons(PORT_NUM);
//	addrSrv.sin_family = AF_INET;
//	// 绑定
//	int len = sizeof(SOCKADDR);
//	// 绑定套接字到本地IP地址，端口号9190
//	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
//	{
//		printf("bind ERRORnum = %d\n", GetLastError());
//		return -1;
//	}
//
//	// 开始监听
//	if (listen(sockSrv, 5) == SOCKET_ERROR)
//	{
//		printf("listen ERRORnum = %d\n", GetLastError());
//		return -1;
//	}
//	printf("start listen...\n");
//	// 新建客户套接字地址供接收
//	SOCKADDR_IN addrCli;
//	// 新建互斥体
//	hMutex = CreateMutex(NULL, FALSE, NULL);
//
//	// 接收并发送
//	while (true)
//	{
//		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrCli, &len);
//
//		// 加入连接套接字组
//		WaitForSingleObject(hMutex, INFINITE);
//		sockConns[connCnt++] = sockConn;
//		ReleaseMutex(hMutex);
//		// 开始套接字函数的线程,传入连接参数
//		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, hConn, &sockConn, 0, NULL);
//
//		printf("Connect client IP: %s\n", inet_ntoa(addrCli.sin_addr));
//		printf("Connect client num: %d\n", connCnt);
//
//#if 0
//		char recvBuf[MAX_BUF];
//		char sendBuf[MAX_BUF];// TODO 初始化
//		recv(sockConn, recvBuf, MAX_BUF, 0);
//		send(sockConn, sendBuf, MAX_BUF, 0);
//		closesocket(sockConn);// 记得关闭!!
//#endif
//		closesocket(sockConn);
//	}
//	// 关闭并清理资源
//	CloseHandle(hMutex);
//	
//	closesocket(sockSrv);
//WSACleanup();
//	return 0;
//}

//多线程+socket编程的一个联合使用
//用互斥体进行线程同步  socket编程   临界区   全局变量

#include <WinSock2.h>
#include <iostream>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLNT 256
#define MAX_BUF_SIZE 256


SOCKET clntSocks[MAX_CLNT];  //所有的连接的客户端socket
HANDLE hMutex;
int clntCnt = 0;  //当前连接的数目

// 服务端的设计：
// 1 每来一个连接，服务端起一个线程（安排一个工人）维护
// 2 将收到的消息转发给所有的客户端
// 3 某个连接断开，需要处理断开的连接

//2 遍历发送给所有的客户端
void SendMsg(char* szMsg, int iLen)
{
	int i = 0;
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)
	{
		send(clntSocks[i], szMsg, iLen, 0);
	}
	ReleaseMutex(hMutex);
}


//处理客户端连接的函数
unsigned WINAPI HandleCln(void* arg)
{
	//1 接收传递过来的参数
	SOCKET hClntSock = *((SOCKET*)arg);
	int iLen = 0, i;
	char szMsg[MAX_BUF_SIZE] = { 0 };
	//2 进行数据的收发  循环接收
	//接收到客户端的数据

//  	while ((iLen = recv(hClntSock, szMsg, sizeof(szMsg),0)) != 0)
//  	{ 		//收到的数据立马发给所有的客户端
//  		SendMsg(szMsg, iLen);
//  	}

	while (1)
	{
		iLen = recv(hClntSock, szMsg, sizeof(szMsg), 0);
		if (iLen != -1)
		{
			//收到的数据立马发给所有的客户端
			SendMsg(szMsg, iLen);
		}
		else
		{
			break;
		}
	}

	printf("此时连接数目为 %d\n", clntCnt);

	//3 某个连接断开，需要处理断开的连接  遍历
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)
	{
		if (hClntSock == clntSocks[i])
		{
			//移位
			while (i++ < clntCnt)
			{
				clntSocks[i] = clntSocks[i + 1];
			}
			break;
		}
	}
	clntCnt--;  //当前连接数的一个自减
	printf("断开此时连接数目 %d", clntCnt);
	ReleaseMutex(hMutex);
	closesocket(hClntSock);
	return 0;
}


int main()
{
	// 加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	HANDLE hThread;
	wVersionRequested = MAKEWORD(1, 1);
	// 初始化套接字库
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return err;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	//创建一个互斥对象
	hMutex = CreateMutex(NULL, FALSE, NULL);
	// 新建套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9190);

	// 绑定套接字到本地IP地址，端口号9190
	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		printf("bind ERRORnum = %d\n", GetLastError());
		return -1;
	}

	// 开始监听
	if (listen(sockSrv, 5) == SOCKET_ERROR)
	{
		printf("listen ERRORnum = %d\n", GetLastError());
		return -1;
	}

	printf("start listen\n");

	SOCKADDR_IN addrCli;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		// 接收客户连接  sockConn此时来的客户端连接
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrCli, &len);

		//每来一个连接，服务端起一个线程（安排一个工人）维护客户端的连接
		//每来一个连接，全局数组应该加一个成员，最大连接数加1
		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = sockConn;
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleCln,
			(void*)&sockConn, 0, NULL);
		printf("Connect client IP: %s \n", inet_ntoa(addrCli.sin_addr));
		printf("Connect client num: %d \n", clntCnt);
	}
	CloseHandle(hMutex);
	closesocket(sockSrv);
	WSACleanup();

	return 0;
}