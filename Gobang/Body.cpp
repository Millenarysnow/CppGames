#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <winsock2.h>
#include <stdio.h>
#include <graphics.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    initgraph(630, 630);
    setbkcolor(WHITE);
    cleardevice();
    setlinecolor(RED);
    
    // 初始化库
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa); 
    
    // 创建用于通信的套接字
    SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        return -1;
    }
    
    // 连接服务器IP Port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(87654);
    char IP[] = "47.116.37.143";
    saddr.sin_addr.S_un.S_addr = inet_addr( IP );
    int ret = connect(fd, (struct sockaddr*)&saddr, sizeof saddr);
    if (ret == -1)
    {
        perror("connect");
        return -1;
    }

    // 通信


    return 0;
}