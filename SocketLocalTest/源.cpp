#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <WinSock2.h>
#include <iostream>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	// 初始化winsock库
	WSADATA data;
	int test = 0;
	test = WSAStartup(MAKEWORD(2, 2), &data);
	if (test != 0)
	{
		perror("startup");
		return -1;
	}

	// 创建用于通信的套接字
	SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket");
		return -1;
	}

	// 连接服务器IP 端口
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(87878);
	saddr.sin_addr.S_un.S_addr = inet_addr("47.116.37.143");
	int ret = connect(fd, (struct sockaddr*)&saddr, sizeof saddr);
	if (ret == -1)
	{
		perror("connect");
		return -1;
	}

	// 通信
	int num = 0;
	while (1)
	{
		// 发送数据
		char buff[1024];
		sprintf(buff, "你好，世界, %d\n", num++);
		send(fd, buff, strlen(buff) + 1, 0);

		// 接收数据
		memset(buff, 0, sizeof buff);
		int len = recv(fd, buff, sizeof buff, 0); // recv为阻塞函数
		if (len > 0)
		{
			cout << "server say:" << buff << endl;
		}
		else if (len == 0)
		{
			cout << "Server break" << endl;
		}
		else
		{
			perror("recv");
			return -1;
		}
		Sleep(1);
	}

	// 关闭文件描述符
	closesocket(fd);

	// 释放套接字库
	WSACleanup();

	return 0;
}