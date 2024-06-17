#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h> // 包含了inet.h

// 初始连接 - 60006
// 玩家1 - 60001
// 玩家2 - 60002

int init(int cnt) // 初始连接
{
	// 创建监听套接字
	int fdo = socket(AF_INET, SOCK_STREAM, 0);
	if (fdo == -1)
	{
		perror("socket");
		return -1;
	}

	// 设置端口复用
	int opt =1;
	setsockopt(fdo, SOL_SOCKET, SO_REUSEADDR,& opt, sizeof(opt));

	// 绑定本地IP端口 - 60006
	struct sockaddr_in saddro;
	saddro.sin_family = AF_INET;
	saddro.sin_port = htons(60006);
	saddro.sin_addr.s_addr = INADDR_ANY;
	int reto = bind(fdo, (struct sockaddr*)&saddro, sizeof saddro);
	if (reto == -1)
	{
		perror("bind");
		return -1;
	}

	// 设置监听
	reto = listen(fdo, 128);
	if (reto == -1)
	{
		perror("listen");
		return -1;
	}

	// 阻塞并等待客户端连接
	struct sockaddr_in caddro;
	int addrleno = sizeof caddro;
	int cfdo = accept(fdo, (struct sockaddr*)&caddro, &addrleno);
	if (cfdo == -1)
	{
		perror("accept");
		return -1;
	}

	// 回传连接用端口末位数
	char lstpt[10];
	sprintf(lstpt, "%d", cnt);
	int lenlstpt = sizeof lstpt;
	send(cfdo, lstpt, lenlstpt, 0);

	// 回收套接字
	close(fdo);
	close(cfdo);
}

int main()
{
	// 初始连接1
	int retn = init(1);
	if (retn == -1)
	{
		perror("init_1");
		return -1;
	}

	// 连接玩家1

	// 创建监听套接字
	int fd1 = socket(AF_INET, SOCK_STREAM, 0);
	if (fd1 == -1)
	{
		perror("socket");
		return -1;
	}

	// 绑定本地IP端口 - 60001
	struct sockaddr_in saddr1;
	saddr1.sin_family = AF_INET;
	saddr1.sin_port = htons(60001);
	saddr1.sin_addr.s_addr = INADDR_ANY;
	int ret1 = bind(fd1, (struct sockaddr*)&saddr1, sizeof saddr1);
	if (ret1 == -1)
	{
		perror("bind");
		return -1;
	}

	// 设置监听
	ret1 = listen(fd1, 128);
	if (ret1 == -1)
	{
		perror("listen");
		return -1;
	}

	// 阻塞并等待客户端连接
	struct sockaddr_in caddr1;
	int addrlen1 = sizeof caddr1;
	int cfd1 = accept(fd1, (struct sockaddr*)&caddr1, &addrlen1);
	if (cfd1 == -1)
	{
		perror("accept");
		return -1;
	}

	// 若连接成功，打印客户端的IP与端口信息
	char ip1[32];
	printf("1客户端IP: %s, 端口: %d\n", inet_ntop(AF_INET, &caddr1.sin_addr.s_addr, ip1, sizeof ip1), ntohs(caddr1.sin_port));

	// 向1发送等待信息
	char answtype[2] = "2";
	int lenansw1 = sizeof answtype;
	send(cfd1, answtype, lenansw1, 0);
	sleep(1);
	char answ[] = "Wait for Another player.";
	int lenansw = sizeof answ;
	send(cfd1, answ, lenansw, 0);

	// 初始连接2
	retn = init(2);
	if (retn == -1)
	{
		perror("init_2");
		return -1;
	}

	// 连接玩家2

	// 创建监听套接字
	int fd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (fd2 == -1)
	{
		perror("socket2");
		return -1;
	}

	// 绑定本地IP端口 - 60002
	struct sockaddr_in saddr2;
	saddr2.sin_family = AF_INET;
	saddr2.sin_port = htons(60002);
	saddr2.sin_addr.s_addr = INADDR_ANY;
	int ret2 = bind(fd2, (struct sockaddr*)&saddr2, sizeof saddr2);
	if (ret2 == -1)
	{
		perror("bind2");
		return -1;
	}

	// 设置监听
	ret2 = listen(fd2, 128);
	if (ret2 == -1)
	{
		perror("listen2");
		return -1;
	}

	// 阻塞并等待客户端连接
	struct sockaddr_in caddr2;
	int addrlen2 = sizeof caddr2;
	int cfd2 = accept(fd2, (struct sockaddr*)&caddr2, &addrlen2);
	if (cfd2 == -1)
	{
		perror("accept2");
		return -1;
	}

	// 若连接成功，打印客户端的IP与端口信息
	char ip2[32];
	printf("2客户端IP: %s, 端口: %d\n", inet_ntop(AF_INET, &caddr2.sin_addr.s_addr, ip2, sizeof ip2), ntohs(caddr2.sin_port));

	// 向1发送游戏开始指令 - 游戏开始
	answtype[1] = '2';
	send(cfd1, answtype, lenansw1, 0);
	sleep(1);
	char answ2[] = "Game start.";
	int lenansw2 = sizeof answ2;
	send(cfd1, answ2, lenansw2, 0);

	// 通信
	char buff[50];
	char num[2];
	int tar = 0; // 规定首先连接为1；后连接为0
	while (1)
	{
		memset(buff, 0, sizeof buff);
		memset(num, 0, sizeof num);

		// 接收数据
		tar ^= 1;

		// 给玩家2发信息
		if (tar)
		{
			int len = recv(cfd1, buff, sizeof buff, 0);
			int lennum = sizeof num;
			if (len > 0)
			{
				sprintf(num, "%c", '0');
				send(cfd2, num, lennum, 0);
				sleep(1);
				send(cfd2, buff, len, 0);
			} // 接收到数据
			else if (len == 0)
			{
				sprintf(num, "%c", '1');
				send(cfd2, num, lennum, 0);
				sleep(1);
				sprintf(buff, "%s", "Another player disconnected.");
				send(cfd2, buff, len, 0);
				break;
			} // 客户端断开连接
			else
			{
				perror("recv");
				break;
			} //调用失败
		}

		// 给玩家1发信息
		else
		{
			int len = recv(cfd2, buff, sizeof buff, 0);
			int lennum = sizeof num;
			if (len > 0)
			{
				sprintf(num, "%c", '0');
				send(cfd1, num, lennum, 0);
				sleep(1);
				send(cfd1, buff, len, 0);
			} // 接收到数据
			else if (len == 0)
			{
				sprintf(num, "%c", '1');
				send(cfd1, num, lennum, 0);
				sleep(1);
				sprintf(buff, "%s", "Another player disconnected.");
				send(cfd1, buff, len, 0);
				break;
			} // 客户端断开连接
			else
			{
				perror("recv");
				break;
			} //调用失败
		}
	}


	// 关闭文件描述符
	close(fd1);
	close(cfd1);
	close(fd2);
	close(cfd2);

	return 0;
}
