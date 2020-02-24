////socket: tcp
////��������߳�ͬ��
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
//// ����˵���ƣ�
//// 1 ÿ��һ�����ӣ��������һ���̣߳�����һ�����ˣ�ά��
//// 2 ���յ�����Ϣת�������еĿͻ���
//// 3 ĳ�����ӶϿ�����Ҫ����Ͽ�������
//
//// ȫ�ֱ���:�ǵû���
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
//	// �յ���Ϣ�����������ӿͻ���
//	char recvBuf[MAX_BUF] = { 0 };
//	int nRecv;
//	while (nRecv = recv(sockConn, recvBuf, MAX_BUF, 0) != 0)
//	{
//		sendMsg(recvBuf, nRecv);
//	}
//
//	// �������ӶϿ�
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
//			std::cout << "�Ͽ���, ��ʱ������Ŀ:" << connCnt << std::endl;
//			break;
//		}
//	}
//	ReleaseMutex(hMutex);
//	return 0;
//}
//
//int main()
//{
//	// �����׽��ֿ�
//	WORD wVersionRequested = MAKEWORD(1, 1);
//	WSADATA wsaData;
//	INT err = WSAStartup(wVersionRequested, &wsaData);
//	if (err != 0)
//	{
//		return err;
//	}
//	if (LOBYTE(wsaData.wVersion) != 1
//		|| HIBYTE(wsaData.wVersion) != 1) {
//		WSACleanup();//�ǵô�������������Դ
//		return -1;
//	}
//	// �½��׽��ֲ���ʼ��
//	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
//	// �½���ַ
//	SOCKADDR_IN addrSrv;
//	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//	addrSrv.sin_port = htons(PORT_NUM);
//	addrSrv.sin_family = AF_INET;
//	// ��
//	int len = sizeof(SOCKADDR);
//	// ���׽��ֵ�����IP��ַ���˿ں�9190
//	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
//	{
//		printf("bind ERRORnum = %d\n", GetLastError());
//		return -1;
//	}
//
//	// ��ʼ����
//	if (listen(sockSrv, 5) == SOCKET_ERROR)
//	{
//		printf("listen ERRORnum = %d\n", GetLastError());
//		return -1;
//	}
//	printf("start listen...\n");
//	// �½��ͻ��׽��ֵ�ַ������
//	SOCKADDR_IN addrCli;
//	// �½�������
//	hMutex = CreateMutex(NULL, FALSE, NULL);
//
//	// ���ղ�����
//	while (true)
//	{
//		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrCli, &len);
//
//		// ���������׽�����
//		WaitForSingleObject(hMutex, INFINITE);
//		sockConns[connCnt++] = sockConn;
//		ReleaseMutex(hMutex);
//		// ��ʼ�׽��ֺ������߳�,�������Ӳ���
//		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, hConn, &sockConn, 0, NULL);
//
//		printf("Connect client IP: %s\n", inet_ntoa(addrCli.sin_addr));
//		printf("Connect client num: %d\n", connCnt);
//
//#if 0
//		char recvBuf[MAX_BUF];
//		char sendBuf[MAX_BUF];// TODO ��ʼ��
//		recv(sockConn, recvBuf, MAX_BUF, 0);
//		send(sockConn, sendBuf, MAX_BUF, 0);
//		closesocket(sockConn);// �ǵùر�!!
//#endif
//		closesocket(sockConn);
//	}
//	// �رղ�������Դ
//	CloseHandle(hMutex);
//	
//	closesocket(sockSrv);
//WSACleanup();
//	return 0;
//}

//���߳�+socket��̵�һ������ʹ��
//�û���������߳�ͬ��  socket���   �ٽ���   ȫ�ֱ���

#include <WinSock2.h>
#include <iostream>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLNT 256
#define MAX_BUF_SIZE 256


SOCKET clntSocks[MAX_CLNT];  //���е����ӵĿͻ���socket
HANDLE hMutex;
int clntCnt = 0;  //��ǰ���ӵ���Ŀ

// ����˵���ƣ�
// 1 ÿ��һ�����ӣ��������һ���̣߳�����һ�����ˣ�ά��
// 2 ���յ�����Ϣת�������еĿͻ���
// 3 ĳ�����ӶϿ�����Ҫ����Ͽ�������

//2 �������͸����еĿͻ���
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


//����ͻ������ӵĺ���
unsigned WINAPI HandleCln(void* arg)
{
	//1 ���մ��ݹ����Ĳ���
	SOCKET hClntSock = *((SOCKET*)arg);
	int iLen = 0, i;
	char szMsg[MAX_BUF_SIZE] = { 0 };
	//2 �������ݵ��շ�  ѭ������
	//���յ��ͻ��˵�����

//  	while ((iLen = recv(hClntSock, szMsg, sizeof(szMsg),0)) != 0)
//  	{ 		//�յ����������������еĿͻ���
//  		SendMsg(szMsg, iLen);
//  	}

	while (1)
	{
		iLen = recv(hClntSock, szMsg, sizeof(szMsg), 0);
		if (iLen != -1)
		{
			//�յ����������������еĿͻ���
			SendMsg(szMsg, iLen);
		}
		else
		{
			break;
		}
	}

	printf("��ʱ������ĿΪ %d\n", clntCnt);

	//3 ĳ�����ӶϿ�����Ҫ����Ͽ�������  ����
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)
	{
		if (hClntSock == clntSocks[i])
		{
			//��λ
			while (i++ < clntCnt)
			{
				clntSocks[i] = clntSocks[i + 1];
			}
			break;
		}
	}
	clntCnt--;  //��ǰ��������һ���Լ�
	printf("�Ͽ���ʱ������Ŀ %d", clntCnt);
	ReleaseMutex(hMutex);
	closesocket(hClntSock);
	return 0;
}


int main()
{
	// �����׽��ֿ�
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	HANDLE hThread;
	wVersionRequested = MAKEWORD(1, 1);
	// ��ʼ���׽��ֿ�
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
	//����һ���������
	hMutex = CreateMutex(NULL, FALSE, NULL);
	// �½��׽���
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9190);

	// ���׽��ֵ�����IP��ַ���˿ں�9190
	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		printf("bind ERRORnum = %d\n", GetLastError());
		return -1;
	}

	// ��ʼ����
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
		// ���տͻ�����  sockConn��ʱ���Ŀͻ�������
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrCli, &len);

		//ÿ��һ�����ӣ��������һ���̣߳�����һ�����ˣ�ά���ͻ��˵�����
		//ÿ��һ�����ӣ�ȫ������Ӧ�ü�һ����Ա�������������1
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