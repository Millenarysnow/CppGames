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

pair<short, short> mp[4][4]; // 棋子类型 剩余存在步数

void DrawMap()
{
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);

	line(0, 600, 600, 600);
}

int judge() // 有人获胜 - 返回获胜人的编号； 无 - 返回2
{
	for (int i = 0; i < 3; i++) // 判断行
	{
		if (
			mp[i][0].second &&
			mp[i][1].second &&
			mp[i][2].second &&
			mp[i][0].first ==
			mp[i][1].first &&
			mp[i][1].first ==
			mp[i][2].first)
			return mp[i][0].first;
	}
	for (int i = 0; i < 3; i++) // 判断列
	{
		if (
			mp[0][i].second &&
			mp[1][i].second &&
			mp[2][i].second &&
			mp[0][i].first ==
			mp[1][i].first &&
			mp[1][i].first ==
			mp[2][i].first)
			return mp[0][i].first;
	}
	for (int i = 0; i < 3; i++) // 主对角线方向 y = x + b
	{
		if (
			mp[0][i % 3].second &&
			mp[1][(i + 1) % 3].second &&
			mp[2][(i + 2) % 3].second &&
			mp[0][i % 3].first ==
			mp[1][(i + 1) % 3].first &&
			mp[1][(i + 1) % 3].first ==
			mp[2][(i + 2) % 3].first
			)
			return mp[0][i % 3].first;
	}

	for (int i = 2; i < 5; i++) // 副对角线方向 y = -x + b(2 -> 4)
	{
		if (
			mp[0][i % 3].second &&
			mp[1][(i - 1) % 3].second &&
			mp[2][(i - 2) % 3].second &&
			mp[0][i % 3].first ==
			mp[1][(i - 1) % 3].first &&
			mp[1][(i - 1) % 3].first ==
			mp[2][(i - 2) % 3].first
			)
			return mp[0][i % 3].first;
	}

	return 2;
}

int GetPort() // 获得通信端口
{
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
	saddr.sin_port = htons(60006);
	inet_pton(AF_INET, "47.116.37.143", &saddr.sin_addr.S_un.S_addr);
	int ret = connect(fd, (sockaddr*)&saddr, sizeof saddr);
	if (ret == -1)
	{
		perror("connect");
		return -1;
	}

	// 获取端口
	char num[10];
	int len = recv(fd, num, sizeof num, 0);

	// 关闭套接字
	closesocket(fd);

	return (num[0] - '0');
}

int main()
{
	// 初始化画布与线型等 - 600 * 700
	initgraph(600, 700);
	setbkcolor(DARKGRAY);
	cleardevice();
	LINESTYLE lsy;
	getlinestyle(&lsy);
	lsy.thickness = 10;
	setlinestyle(&lsy);
	setlinecolor(WHITE);
	TCHAR tips[] = _T("Connected for server.");
	outtextxy(30, 350, tips);

	// 初始化winsock库
	WSADATA data;
	int test = 0;
	test = WSAStartup(MAKEWORD(2, 2), &data);
	if (test != 0)
	{
		perror("startup");
		return -1;
	}

	// 获取连接端口末位数 - 6000x
	int lst = GetPort();

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
	saddr.sin_port = htons((60000 + lst));
	inet_pton(AF_INET, "47.116.37.143", &saddr.sin_addr.S_un.S_addr);
	int ret = connect(fd, (sockaddr*)&saddr, sizeof saddr);
	if (ret == -1)
	{
		perror("connect");
		return -1;
	}

	// 通信
	char num[2]; // 定义： 0 - 游戏数据； 1 - 需要输出的文字
	char buff[100]; // 若buff传入游戏数据：
					// 定义：【0】 - 棋子x；【1】 - 棋子y
					// 0 - 圆形； 1 - 矩形
					// 定义 0 为己方颜色
	char buffout[100]; // 传出游戏数据
	ExMessage message;
	short relx = 0, rely = 0; // 己方
	short anox = 0, anoy = 0; // 敌方

	cleardevice();

	while (1)
	{
		// 接收数据
		memset(buff, 0, sizeof buff);
		memset(num, 0, sizeof num);
		int len = recv(fd, num, sizeof num, 0);
		if (len > 0)
		{
			clearrectangle(0, 660, 600, 700);
			if (num[0] == '1') // 该信息需要打印
			{
				recv(fd, buff, sizeof buff, 0);
				outtextxy(30, 650, buff);
			}
			else if (num[0] == '2') // 该信息为首个 - 需要等待玩家2
			{
				recv(fd, buff, sizeof buff, 0);
				outtextxy(30, 650, buff);
				memset(buff, 0, sizeof buff);
				recv(fd, buff, sizeof buff, 0);
				clearrectangle(0, 660, 600, 700);
				outtextxy(30, 650, buff);
			}
			else if (num[0] == '0') // 该信息为游戏信息
			{
				recv(fd, buff, sizeof buff, 0);
				anox = buff[0] - '0'; // 对方x
				anoy = buff[1] - '0'; // 对方y
				mp[anox][anoy] = { 1, 4 };
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

		// 绘制已存在的棋子
		DrawMap();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				if (mp[i][j].second)
					mp[i][j].second--;
				if (mp[i][j].second)
				{
					if (!mp[i][j].first) // 0 - 圆形
						fillcircle(i * 200 + 100, j * 200 + 100, 70);
					else
						fillrectangle(i * 200 + 30, j * 200 + 30, i * 200 + 170, j * 200 + 170);
				}
			}

		// 获取鼠标信息
		while (1)
		{
			message = getmessage();
			if (message.message == WM_LBUTTONDOWN)
			{
				relx = message.x / 200;
				rely = message.y / 200;
				break;
			}
		}
		mp[relx][rely] = { 0, 3 };
		fillcircle(relx * 200 + 100, rely * 200 + 100, 70);

		// 判断是否有人获胜
		int jud = judge();
		if (jud == 0) // 己方获胜
		{
			char gmover[] = "You Win!";
			outtextxy(30, 650, gmover);
			break;
		}
		else if (jud == 1) // 敌方获胜
		{
			char gmover[] = "You loose!";
			outtextxy(30, 650, gmover);
			break;
		}

		// 发送数据
		sprintf(buffout, "%d%d", relx, rely);
		send(fd, buffout, strlen(buffout) + 1, 0);
	}

	// 关闭文件描述符
	closesocket(fd);

	// 释放套接字库
	WSACleanup();

	return 0;
}