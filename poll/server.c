/*************************************************************************************
 * 服务器无法向客户端发送信息，如需要该功能要添加，（对连接的客户端的套接字取名，以方便发送数据）
 * poll
 * **********************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
 
#define NFDS 100//fds数组的大小
 
// 创建一个用于监听的socket  
int CreateSocket()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(-1 != listenfd);
 
	struct sockaddr_in ser;
	memset(&ser, 0, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6000);                         //端口号
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");       //ID
 
	int res =  bind(listenfd, (struct sockaddr*)&ser, sizeof(ser));       //绑定端口
	assert(-1 != res); 
 
	listen(listenfd, 5);               //转换为监听套接字
 
	return listenfd;                   //返回监听套接字描述符
}
 
// 初始化fds结构体数组
void InitFds(struct pollfd *fds)
{
	int i = 0;
	for(; i < NFDS; ++i)
	{
		fds[i].fd = -1;
		fds[i].events = 0;
		fds[i].revents = 0;
	}
}
 
// 向fds结构体数组中插入一个文件描述符
void InsertFd(struct pollfd *fds, int fd, int flag)//此处flag是为了判断是文件描述符c，还是listenfd，来设置events
{
	int i = 0;
	for(; i < NFDS; ++i)
	{
		if(fds[i].fd == -1)
		{
			fds[i].fd = fd;
			fds[i].events |= POLLIN;             //数据可读
			if(flag)
			{
				fds[i].events |= POLLRDHUP;     //TCP连接被对端关闭，或者关闭了写操作，由GNU引入
			}
 
			break;
		}
	}
}
 
// 从fds结构体数组中删除一个文件描述符
void DeleteFd(struct pollfd *fds, int fd)
{
	int i = 0;
	for(; i < NFDS; ++i)
	{
		if(fds[i].fd == fd)
		{
			fds[i].fd = -1;
			fds[i].events = 0; 
			break;
		}
	}
}
 
// 获取一个已完成三次握手的连接
void GetClientLink(int fd, struct pollfd *fds)
{
	struct sockaddr_in cli;
	socklen_t len = sizeof(cli); 
	int c = accept(fd, (struct sockaddr*)&cli, &len);   //接受到一个套接字
	assert(c != -1);
 
	printf("one client link success\n");
 
	InsertFd(fds, c, 1);          //加入结构体数组fds中
}
 
// 断开一个用户连接
void UnlinkClient(int fd, struct pollfd *fds)
{
	close(fd);
	DeleteFd(fds, fd);            //从fds结构体数组中删除一个文件描述符
	printf("one client unlink\n");
}
 
// 处理客户端发送来的数据
void DealClientData(int fd, struct pollfd *fds)
{
	char  buff[128] = {0};
 
	int n = recv(fd, buff, 127, 0);
	if(n <= 0)
	{
		UnlinkClient(fd, fds);
		return;
	}
 
	printf("%s\n", buff);
 
	send(fd, "send seccess", 2, 0);
}
 
// poll返回后，处理就绪的文件描述符
void DealFinishFd(struct pollfd *fds, int listenfd)
{
	int i = 0;
	for(; i < NFDS; ++i)
	{
		if(fds[i].fd == -1)
		{
			continue;
		}
 
		int fd = fds[i].fd;
		if(fd == listenfd && fds[i].revents & POLLIN)    //监听到连接套接字，加入到poll中
		{
			GetClientLink(fd, fds);
			//获取连接
		}
		else if(fds[i].revents & POLLRDHUP)        //对端关闭该套接字
		{
			UnlinkClient(fd, fds);
			//断开连接
		}
		else if(fds[i].revents & POLLIN)    
		{
			DealClientData(fd, fds);
			//处理客户端数据
		}
	}
}
 
int main()
{
	int listenfd = CreateSocket();     //创建一个用于监听的socket
 
	struct pollfd *fds = (struct pollfd*)malloc(sizeof(struct pollfd) * NFDS);
	//malloc一个fds结构体数组
	assert(NULL != fds);
 
	InitFds(fds);
	//初始化fds结构体数组
 
	InsertFd(fds, listenfd, 0);
	//插入文件描述符listenfd
 
 
	while(1)
	{
		int n = poll(fds, NFDS, -1);       //
		if(n <= 0)
		{
			printf("poll error\n");
			continue;
		}
 
		DealFinishFd(fds, listenfd);
		//处理就绪的文件描述符
	}
 
	free(fds);
}