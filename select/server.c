#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> //atoi()
 
int get_max_socket_fd(int socket_arr[]);//获取最大的通信socket描述符。
 
int main(int argc, char *argv[])
{
	//判断命令行参数是否满足
	if(argc != 2)
	{
		printf("请传递一个端口号\n");
		return -1;
	}
 
	//将接收端口号并转换为int
	int port = atoi(argv[1]);
	if( port<1025 || port>65535 )//0~1024一般给系统使用，一共可以分配到65535
	{
		printf("端口号范围应为1025~65535");
		return -1;
	}
 
	//1 创建tcp通信socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1)
	{
		perror("创建tcp通信socket失败!\n");
		return -1;
	}
 
	//2 绑定socket地址
	struct sockaddr_in server_addr = {0};//存放地址信息
	server_addr.sin_family = AF_INET;//AF_INET->IPv4  
	server_addr.sin_port = htons(port);//端口号
	server_addr.sin_addr.s_addr = INADDR_ANY;//让系统检测本地网卡，自动绑定本地IP
	int ret = bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr) );
	if(ret == -1)
	{
		perror("bind failed!\n");
		return -1;
	}
 
	//3 设置监听队列，设置为可以接受5个客户端连接
	ret = listen(socket_fd, 5);
	if(ret == -1)
	{
		perror("listen falied!\n");
	}
 
 
	printf("server is running!\n");
 
	
	struct sockaddr_in client_addr = {0};//用来保存客户端的地址信息
	int len = sizeof(client_addr);
	int socket_fd_arr[5] = {-1, -1, -1, -1, -1};//用来保存5个客户端的通信socket
	
 
	//定义一个文件描述符集合
	fd_set fds;
	//定义一个时间结构体  
	struct timeval  time;
	time.tv_sec = 3;//超时时间
	time.tv_usec = 0;
	//循环监视文件描述符集合 
	int new_socket_fd = -1;
	int max_socket_fd = -1;//保存最大的socket描述符
	int index = -1, i;
	
	//循环监视文件描述符集合
	while(1)
	{
		//清空文件描述符集合
		FD_ZERO(&fds);
		//把标准输入设备加入到集合中 
		FD_SET(0, &fds);
		FD_SET(socket_fd, &fds);//将监听socket添加到集合中
		
		//把网络通信文件描述符加入到集合中 
		for(i=0; i<=index; i++)
		{
			FD_SET(socket_fd_arr[i],&fds);
		}
		
		//获取最大的通信socket描述符。
		max_socket_fd = get_max_socket_fd(socket_fd_arr);
		if(max_socket_fd == -1)
		{
			max_socket_fd = socket_fd;
		}
		
		ret = select(max_socket_fd+1,&fds,NULL,NULL,&time);//检查集合中是否有活跃的描述符
		if(ret < 0)//错误
		{
			perror("select fail");
			return -1;
		}
		else if(ret == 0) //超时
		{
			//printf("timeout1\n");
		}
		else if(ret > 0) //有活跃的 
		{
			if(FD_ISSET(socket_fd,&fds))//监听socket活跃，说明有客户端请求连接
			{
				new_socket_fd = accept( socket_fd, (struct sockaddr *)&client_addr, &len);
				if(new_socket_fd == -1)
				{
					perror("accpet error!\n");
					continue;
				}
				else
				{
					if(index>=5)
					{
						index = 4;
						printf("index>=5\n");
						continue;
					}
					socket_fd_arr[++index] = new_socket_fd;//将通信socket保存到数组中
					printf("[ID:%d] IP:%s, PORT:%d [connected]\n", index, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					
				}
			}
			
			//判断是否 标准输入设备活跃 假设是则发送数据 
			if(FD_ISSET(0,&fds))
			{
				char buf[1024] = {0};
				int client = -1;
				scanf("%d %s", &client, buf);
				if(client>=0 && client<=index)
				{
					write(socket_fd_arr[client],buf,strlen(buf));
					
				}
				else //给所有的客户端发消息
				{
					for(i=0; i<=index; i++)
					{
						write(socket_fd_arr[i],buf,strlen(buf));
					}
			
				}
				if(strcmp(buf, "exit") == 0)
				{
					break;
				}
			}
			
			//判断是否有收到消息 
			for(i=0; i<=index; i++)
			{
				if(FD_ISSET(socket_fd_arr[i],&fds))//判断通信socket是否有活跃
				{
					char buf[1024] = {0};
					read(socket_fd_arr[i], buf, sizeof(buf));        //读出数据
					
					if(strcmp(buf, "exit") == 0 || strcmp(buf, "") == 0)
					{
						break;
					}
					else if(strlen(buf)>0)
					{
						printf("receive msg form [ID:%d]:%s\n", i, buf);      //打出数据
					}
				}
			}
		}
		
	}
 
	//5 关闭通信socket
	close(new_socket_fd);
	close(socket_fd);
 
	return 0;
}
 
//获取最大的通信socket描述符。
int get_max_socket_fd(int socket_arr[])
{
	int max = -1;
	int i;
	for(i=0; i<5; i++)
	{
		if(socket_arr[i]>max)
		{
			max = socket_arr[i];
		}
	}
	
	return max;
}