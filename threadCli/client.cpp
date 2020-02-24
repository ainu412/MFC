// 1  接收服务端的消息   安排一个工人 起一个线程接收消息
// 2 发送消息给服务端    安排一个工人 起一个线程发送消息
// 3 退出机制

#include <WinSock2.h>
#include <iostream>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define NAME_SIZE 32
#define BUF_SIZE 256

char szName[NAME_SIZE] = "[DEFAULT]";
char szMsg[BUF_SIZE];

//发送消息给服务端
unsigned WINAPI SendMsg(void* arg)
{
	//1 接收传递过来的参数
	SOCKET hClntSock = *((SOCKET*)arg);
	char szNameMsg[NAME_SIZE + BUF_SIZE];  //又有名字，又有消息
	//循环接收来自于控制台的消息
	while (1)
	{
		fgets(szMsg, BUF_SIZE, stdin); //阻塞在这一句
		//退出机制  当收到q或Q  退出
		if (!strcmp(szMsg, "Q\n") || !strcmp(szMsg, "q\n"))
		{
			closesocket(hClntSock);
			exit(0);
		}

		sprintf(szNameMsg, "%s %s", szName, szMsg);//字符串拼接
		send(hClntSock, szNameMsg, strlen(szNameMsg), 0);//发送
	}
	return 0;
}

//接收服务端的消息
unsigned WINAPI RecvMsg(void* arg)
{
	//1 接收传递过来的参数
	SOCKET hClntSock = *((SOCKET*)arg);
	char szNameMsg[NAME_SIZE + BUF_SIZE];  //又有名字，又有消息
	int iLen = 0;
	while (1)
	{
		//recv阻塞
		iLen = recv(hClntSock, szNameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
		//服务端断开
		if (iLen == -1)
		{
			return -1;
		}
		// szNameMsg的0到iLen -1 都是收到的数据 iLen个
		szNameMsg[iLen] = 0;
		//接收到的数据输出到控制台
		fputs(szNameMsg, stdout);
	}
	return 0;
}

// 带参数的main函数，用命令行启动  在当前目录按下shift + 鼠标右键 cmd
int main(int argc, char* argv[])
{
	// 加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	SOCKET hSock;
	SOCKADDR_IN servAdr;
	HANDLE hSendThread, hRecvThread;
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
	sprintf(szName, "[%s]", argv[1]);
	// 1 建立socket
	hSock = socket(PF_INET, SOCK_STREAM, 0);

	// 2 配置端口和地址
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAdr.sin_family = AF_INET;
	servAdr.sin_port = htons(9190);

	// 3 连接服务器
	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		printf("connect error error code = %d\n", GetLastError());
		return -1;
	}

	// 4 发送服务端的消息   安排一个工人 起一个线程发送消息

	hSendThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg,
		(void*)&hSock, 0, NULL);

	// 5 接收消息给服务端   安排一个工人 起一个线程接收消息

	hRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg,
		(void*)&hSock, 0, NULL);

	// 6 等待内核对象的信号发生变化
	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	// 7 关闭套接字
	closesocket(hSock);
	WSACleanup();
	return 0;
}
#if 0
#include <process.h>
#include <Windows.h>
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
// 1  接收服务端的消息   安排一个工人 起一个线程接收消息
// 2 发送消息给服务端    安排一个工人 起一个线程发送消息
// 3 退出机制
#define MAX_MSG_BUF 256
#define MAX_NAME_BUF 36
#define PORT_NUM 7000
char* szName;
char* szMsg;
unsigned __stdcall sendMsg(void* arg)
{
	SOCKET sockCli = *(SOCKET*)arg;
	char sendBuf[MAX_NAME_BUF + MAX_MSG_BUF] = { 0 };

	//接收来自控制台的消息
	while (true)
	{
		fgets(szMsg, MAX_MSG_BUF, stdin);
		// 消息结束机制
		if ('q' == *szMsg || 'Q' == *szMsg) {
			break;
		}
		sprintf_s(sendBuf, MAX_NAME_BUF + MAX_MSG_BUF, "%s %s", szName, szMsg);
		send(sockCli, sendBuf, MAX_NAME_BUF + MAX_MSG_BUF, 0);
	}
	return 0;
}

unsigned __stdcall recvMsg(void* arg)
{
	SOCKET sockCli = *(SOCKET*)arg;
	char recvBuf[MAX_NAME_BUF + MAX_MSG_BUF] = { 0 };
	while (true)
	{
		int iLen = recv(sockCli, recvBuf, MAX_NAME_BUF + MAX_MSG_BUF - 1, 0);
		if (iLen == -1)
		{
			return -1;
		}
		recvBuf[iLen] = 0;
		fputs(recvBuf, stdout);
	}
	return 0;
}

int main(int argc, char* argv[])//用户
{
	//加载套接字库
	WORD wVersion;
	WSADATA wsaData;
	int err;
	wVersion = MAKEWORD(1, 1);
	err = WSAStartup(wVersion, &wsaData);
	if (err != 0)
	{
		return err;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	sprintf_s(szName, MAX_NAME_BUF, "[%s]", argv[1]);

	// 新建套接字
	SOCKET sockCli = socket(AF_INET, SOCK_STREAM, 0);
	// 确定连接的服务端地址
	SOCKADDR_IN addrConn;
	addrConn.sin_family = AF_INET;
	addrConn.sin_port = htons(PORT_NUM);
	addrConn.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	// 连接服务端地址
	connect(sockCli, (SOCKADDR*)&addrConn, sizeof(SOCKADDR));

	// 接收与发送消息
	while (TRUE)
	{
		HANDLE hHandle = (HANDLE)_beginthreadex(NULL, 0, sendMsg, &sockCli, 0, NULL);
	}

	// 清理资源
	closesocket(sockCli);
	WSACleanup();

	return 0;
}
#endif
