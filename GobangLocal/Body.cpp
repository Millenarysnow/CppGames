#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <stdio.h>
#include <graphics.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void DrawMap()
{
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);

	line(0, 600, 600, 600);
}

int main()
{
	// 初始化画布与线型等 - 600 * 700
	initgraph(600, 700);
	LINESTYLE lsy;
	getlinestyle(&lsy);
	lsy.thickness = 10;
	setlinestyle(&lsy);
	setlinecolor(WHITE);

	// 初始化winsock库
	WSADATA data;
	int test = 0;
	test = WSAStartup(MAKEWORD(2, 2), &data);
	if (test != 0)
	{
		perror("startup");
		return -1;
	}
	cout << "startup" << endl;

	// 创建用于通信的套接字
	SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket");
		return -1;
	}
	cout << "socket" << endl;

	// 连接服务器IP 端口
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(60001);
	//saddr.sin_addr.S_un.S_addr = inet_addr("47.116.37.143");
	inet_pton(AF_INET, "47.116.37.143", &saddr.sin_addr.S_un.S_addr);
	int ret = connect(fd, (sockaddr*)&saddr, sizeof saddr);
	if (ret == -1)
	{
		perror("connect");
		return -1;
	}
	cout << "connect" << endl;

	// 通信
	char num[2];
	char buff[100];
	ExMessage message;
	short relx = 0, rely = 0;
	while (1)
	{
		//// 发送数据
		//sprintf(buff, "Hello, %d\n", num++);
		//send(fd, buff, strlen(buff) + 1, 0);

		DrawMap();

		// 接收数据
		TCHAR waiting[] = _T("Waiting for the other to act.");
		outtextxy(30, 650, waiting);
		memset(buff, 0, sizeof buff);
		memset(num, 0, sizeof num);
		int len = recv(fd, num, sizeof num, 0); 
		if (len > 0)
		{
			clearrectangle(0, 660, 600, 700);
			if (buff[0] == '1') // 该信息需要打印
			{
				recv(fd, buff, sizeof buff, 0);
				outtextxy(30, 650, buff);
			}
			else if (buff[0] == '0') // 该信息为游戏信息
			{

			}

		
		}
		else if (len == 0)
		{
			cout << "Server break" << endl;
			return -1;
		}
		else
		{
			perror("recv");
			return -1;
		}
	
		while (1) // 获取鼠标信息
		{
			message = getmessage();
			if (message.message == WM_LBUTTONDOWN)
			{
				relx = message.x / 200;
				rely = message.y / 200;
				break;
			}
		}
	}

	// 关闭文件描述符
	closesocket(fd);

	// 释放套接字库
	WSACleanup();

	return 0;
}