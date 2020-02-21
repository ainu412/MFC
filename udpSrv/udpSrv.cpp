// udpSrv.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    // 加载资源
    WORD wVersion;
    WSADATA wsaData;
    int err;//最好分开写,不然可能出错!

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

    // 加载udp套接字,并绑定地址
    SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_port = htons(6001);
    //绑定
    int len = sizeof(SOCKADDR);
    bind(sockSrv, (SOCKADDR*)&addrSrv, len);

    // 创建接收客户地址
    SOCKADDR_IN addrCli;

    std::cout << "Server start at port: 6001" << std::endl;
    char sendBuf[] = "udp server";
    char recvBuf[100];
    while (true)
    {
        //接收信息并打印
        recvfrom(sockSrv, recvBuf, sizeof(recvBuf), 0, (SOCKADDR*)&addrCli, &len);
        std::cout << "Receive from:" << recvBuf << std::endl;

        sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrCli, len);
        std::cout << "Send from:" << sendBuf << std::endl;

    }
    //清理资源
    closesocket(sockSrv);
    WSACleanup();

    //服务器一直工作无法到达清理资源这一步再return 0 故不需暂停
    return 0;
}
