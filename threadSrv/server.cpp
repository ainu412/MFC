//socket: tcp
//��������߳�ͬ��

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

#define MAX_BACKLOG 5
#define MAX_BUF 1024
// ����˵���ƣ�
// 1 ÿ��һ�����ӣ��������һ���̣߳�����һ�����ˣ�ά��
// 2 ���յ�����Ϣת�������еĿͻ���
// 3 ĳ�����ӶϿ�����Ҫ����Ͽ�������

int main()
{
	// �����׽��ֿ�
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
	// �½��׽��ֲ���ʼ��
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	// �½���ַ
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_port = htons(7000);
	addrSrv.sin_family = AF_INET;
	// ��
	int len = sizeof(SOCKADDR);
	bind(sockSrv, (SOCKADDR*)&addrSrv, len);
	// ����
	listen(sockSrv, MAX_BACKLOG);
	// �½��ͻ��׽��ֵ�ַ������
	SOCKADDR_IN addrCli;
	// ���ղ�����
	while (true)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrCli, &len);
		char recvBuf[MAX_BUF];
		char sendBuf[MAX_BUF];// TODO ��ʼ��
		recv(sockConn, recvBuf, MAX_BUF, 0);
		send(sockConn, sendBuf, MAX_BUF, 0);
		closesocket(sockConn);// �ǵùر�!!
	}
	// �رղ�������Դ
	WSACleanup();
	closesocket(sockSrv);

	return 0;
}