/*************************************************************************************
 * 服务器无法向客户端发送信息，如需要该功能要添加，（对连接的客户端的套接字取名，以方便发送数据）
 * epoll
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
#include <sys/epoll.h>
#include <errno.h>
 
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

int main()
{
    int epfd,s;
    struct epoll_event event, wait_event, *events;

	int listenfd = CreateSocket();     //创建一个用于监听的socket
	
    epfd = epoll_create(10);
 
    epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event);       //标准输入

    event.data.fd = listenfd;
    event.events = EPOLLIN | EPOLLET;            //采用ET
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event);

    events = calloc(64, sizeof event);         //返回事件的缓冲区

	while(1)
	{
		int n = epoll_wait(epfd, &wait_event, 2, 1);       //等待事件发生
 
		for(int i = 0;i < n; i++)
        {
			/*
            if ((events[i].events & EPOLLERR) || 
				(events[i].events & EPOLLHUP) || 
				(!(events[i].events & EPOLLIN)))
			{
				//An error has occured on this fd, or the socket is not
				  // ready for reading (why were we notified then?) 
				fprintf(stderr, "epoll error\n");
				close(events[i].data.fd);
				continue;
			}
            else 
			*/
			if(listenfd = events[i].data.fd)    //监听套接字有通知，
            {
                while(1)
                {
                    struct sockaddr_in addr;
                    int infd;
					socklen_t len ;
					len =  sizeof addr;
                    infd = accept(listenfd, (struct sockaddr *)&addr,&len);
                    if (infd == -1)
					{
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
						{
							/* We have processed all incoming connections. */
							break;
						}
						else
						{
							perror("accept");
							break;
						}
					} 
                    //accept获取套接字描述符
                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_ADD,infd, &event);
                }
            }
            else              //读取数据
            {
                int done = 0;
                while(1)
                {
					int count;
                    char buf[1024];
                    count = read(events[i].data.fd, buf, sizeof(buf)); 

					if(count == -1)
					{
						if(errno != EAGAIN)    //errno == EAGAIN，表示我们已经读取所有信息，可以回到主循环
						{
							perror("read");
							done = 1;
						}
						break;
					}
					else if(count == 0)   //文件结束，遥控器已关闭连接
					{
						done = 1;
						break;
					}
					
					printf("from:%d   :%s",events[i].data.fd, buf);

					if(done)
					{
						printf("Closed connection on descriptor %d\n", events[i].data.fd);
						close(events[i].data.fd);
					}
                }
            }
            
        }
	}
 
	free(events);

	close(epfd);

	return 0;
}