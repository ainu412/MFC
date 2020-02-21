#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
int main()
{
    WORD wVersionRequested;
    WSADATA wsadata;
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsadata);
    if (err != 0)
    {
        return err;
    }
    if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
    {
        WSACleanup();
        return -1;
    }
    //创建本机用户套接字
    SOCKET sockCli = socket(AF_INET, SOCK_DGRAM, 0);
    //udp需要准备本机地址
    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//接收地址为本机地址
    addrSrv.sin_port = htons(6001);
    char recvBuf[100];
    char sendBuf[] = "udp client";
    int len = sizeof(SOCKADDR);

    sendto(sockCli, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrSrv, len);
    std::cout << "Send from:" << sendBuf << std::endl;

    recvfrom(sockCli, recvBuf, 100, 0, (SOCKADDR*)&addrSrv, &len);
    std::cout << "Receive from:" << recvBuf << std::endl;

    //清理资源
    closesocket(sockCli);
    WSACleanup();
    system("pause");//用户记得暂停,不然exe执行结束就关闭了
    return 0;
}