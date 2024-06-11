#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h> // 包含了inet.h

int main()
{
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

	char answtype[2] = "1";
	char answ[] = "Wait for Another player.";
	int lenansw = sizeof answ;
	int lenansw1 = sizeof answtype;
	send(cfd1, answtype, lenansw1, 0);
	send(cfd1, answ, lenansw, 0);

	// 连接玩家2

	// 创建监听套接字
	int fd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (fd2 == -1)
	{
		perror("socket2");
		return -1;
	}

	// 绑定本地IP端口 - 60001
	struct sockaddr_in saddr2;
	saddr2.sin_family = AF_INET;
	saddr2.sin_port = htons(60001);
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


	// 通信
	while (1)
	{
		// 接收数据
		char buff[1024];
		int len = recv(cfd1, buff, sizeof buff, 0);
		if (len > 0)
		{
			printf("client say: %s\n", buff);
			send(cfd1, buff, len, 0);
		} // 接收到数据
		else if (len == 0)
		{
			break;
		} // 客户端断开连接
		else
		{
			perror("recv");
			break;
		} //调用失败
	}


	// 关闭文件描述符
	close(fd1);
	close(cfd1);
	close(fd2);
	close(cfd2);

	return 0;
}