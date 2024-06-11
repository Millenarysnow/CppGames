#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h> // 包含了inet.h

int main()
{
	// 创建监听套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket");
		return -1;
	}

	// 绑定本地IP端口 - 60001
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(60001);
	saddr.sin_addr.s_addr = INADDR_ANY; // 该宏实际值为0 - 即0.0.0.0
										// 即该套接字可以绑定本地任意一个IP地址
										// 0没有大小端之分，因此不需要转换
	int ret = bind(fd, (struct sockaddr*)&saddr, sizeof saddr);
	if (ret == -1)
	{
		perror("bind");
		return -1;
	}
	
	// 设置监听
	ret = listen(fd, 128);
	if (ret == -1)
	{
		perror("listen");
		return -1;
	}

	// 阻塞并等待客户端连接
	struct sockaddr_in caddr;
	int addrlen = sizeof caddr;
	int cfd = accept(fd, (struct sockaddr*)&caddr, &addrlen);
	if (cfd == -1)
	{
		perror("accept");
		return -1;
	}

	// 若连接成功，打印客户端的IP与端口信息
	char ip[32];
	printf("客户端IP: %s, 端口: %d\n", inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof ip), ntohs(caddr.sin_port));
	
	// 通信
	while (1)
	{
		// 接收数据
		char buff[1024];
		int len = recv(cfd, buff, sizeof buff, 0);
		if (len > 0)
		{
			
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
	close(fd);
	close(cfd);

	return 0;
}