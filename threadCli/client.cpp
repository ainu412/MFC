// 1  ���շ���˵���Ϣ   ����һ������ ��һ���߳̽�����Ϣ
// 2 ������Ϣ�������    ����һ������ ��һ���̷߳�����Ϣ
// 3 �˳�����

#include <WinSock2.h>
#include <iostream>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define NAME_SIZE 32
#define BUF_SIZE 256

char szName[NAME_SIZE] = "[DEFAULT]";
char szMsg[BUF_SIZE];

//������Ϣ�������
unsigned WINAPI SendMsg(void* arg)
{
	//1 ���մ��ݹ����Ĳ���
	SOCKET hClntSock = *((SOCKET*)arg);
	char szNameMsg[NAME_SIZE + BUF_SIZE];  //�������֣�������Ϣ
	//ѭ�����������ڿ���̨����Ϣ
	while (1)
	{
		fgets(szMsg, BUF_SIZE, stdin); //��������һ��
		//�˳�����  ���յ�q��Q  �˳�
		if (!strcmp(szMsg, "Q\n") || !strcmp(szMsg, "q\n"))
		{
			closesocket(hClntSock);
			exit(0);
		}

		sprintf(szNameMsg, "%s %s", szName, szMsg);//�ַ���ƴ��
		send(hClntSock, szNameMsg, strlen(szNameMsg), 0);//����
	}
	return 0;
}

//���շ���˵���Ϣ
unsigned WINAPI RecvMsg(void* arg)
{
	//1 ���մ��ݹ����Ĳ���
	SOCKET hClntSock = *((SOCKET*)arg);
	char szNameMsg[NAME_SIZE + BUF_SIZE];  //�������֣�������Ϣ
	int iLen = 0;
	while (1)
	{
		//recv����
		iLen = recv(hClntSock, szNameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
		//����˶Ͽ�
		if (iLen == -1)
		{
			return -1;
		}
		// szNameMsg��0��iLen -1 �����յ������� iLen��
		szNameMsg[iLen] = 0;
		//���յ����������������̨
		fputs(szNameMsg, stdout);
	}
	return 0;
}

// ��������main������������������  �ڵ�ǰĿ¼����shift + ����Ҽ� cmd
int main(int argc, char* argv[])
{
	// �����׽��ֿ�
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	SOCKET hSock;
	SOCKADDR_IN servAdr;
	HANDLE hSendThread, hRecvThread;
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
	sprintf(szName, "[%s]", argv[1]);
	// 1 ����socket
	hSock = socket(PF_INET, SOCK_STREAM, 0);

	// 2 ���ö˿ں͵�ַ
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAdr.sin_family = AF_INET;
	servAdr.sin_port = htons(9190);

	// 3 ���ӷ�����
	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		printf("connect error error code = %d\n", GetLastError());
		return -1;
	}

	// 4 ���ͷ���˵���Ϣ   ����һ������ ��һ���̷߳�����Ϣ

	hSendThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg,
		(void*)&hSock, 0, NULL);

	// 5 ������Ϣ�������   ����һ������ ��һ���߳̽�����Ϣ

	hRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg,
		(void*)&hSock, 0, NULL);

	// 6 �ȴ��ں˶�����źŷ����仯
	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	// 7 �ر��׽���
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
// 1  ���շ���˵���Ϣ   ����һ������ ��һ���߳̽�����Ϣ
// 2 ������Ϣ�������    ����һ������ ��һ���̷߳�����Ϣ
// 3 �˳�����
#define MAX_MSG_BUF 256
#define MAX_NAME_BUF 36
#define PORT_NUM 7000
char* szName;
char* szMsg;
unsigned __stdcall sendMsg(void* arg)
{
	SOCKET sockCli = *(SOCKET*)arg;
	char sendBuf[MAX_NAME_BUF + MAX_MSG_BUF] = { 0 };

	//�������Կ���̨����Ϣ
	while (true)
	{
		fgets(szMsg, MAX_MSG_BUF, stdin);
		// ��Ϣ��������
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

int main(int argc, char* argv[])//�û�
{
	//�����׽��ֿ�
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

	// �½��׽���
	SOCKET sockCli = socket(AF_INET, SOCK_STREAM, 0);
	// ȷ�����ӵķ���˵�ַ
	SOCKADDR_IN addrConn;
	addrConn.sin_family = AF_INET;
	addrConn.sin_port = htons(PORT_NUM);
	addrConn.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	// ���ӷ���˵�ַ
	connect(sockCli, (SOCKADDR*)&addrConn, sizeof(SOCKADDR));

	// �����뷢����Ϣ
	while (TRUE)
	{
		HANDLE hHandle = (HANDLE)_beginthreadex(NULL, 0, sendMsg, &sockCli, 0, NULL);
	}

	// ������Դ
	closesocket(sockCli);
	WSACleanup();

	return 0;
}
#endif
