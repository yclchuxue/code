/**************************
 * 客户端
 * ***********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct denn{
	int ice;
	int id;
	char name[10];
	char password[6];
}DENN;


int main()
{
    int n;
	DENN *XX = (DENN*)malloc(sizeof(DENN));
	while(1)	
    {
		printf("/*******************************/\n");
		printf("/**************1 登陆**************/\n");
    	printf("/**************2 注册**************/\n");
	    printf("/************3 找回密码*************/\n");
		printf("/************0 退出程序*************/\n");
	    printf("请选择：");
		scanf("%d",&n);
		if(n == 0)
		{
			exit(0);
		}
		if(n > 0 && n <= 3)
		{

			break;
		}
	}
	XX->ice = n;         //选择功能

	int port = atoi("9999");      //从命令行获取端口号
	if( port<1025 || port>65535 )       //0~1024一般给系统使用，一共可以分配到65535
	{
		printf("端口号范围应为1025~65535");
		return -1;
	}
	
	//1 创建tcp通信socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1)
	{
		perror("socket failed!\n");
	}
 
	//2 连接服务器
	struct sockaddr_in server_addr = {0};//服务器的地址信息
	server_addr.sin_family = AF_INET;//IPv4协议
	server_addr.sin_port = htons(port);//服务器端口号
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");         //设置服务器IP
	int ret = connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));       //连接服务器
	if(ret == -1)
	{
		perror("connect failed!\n");
	}
	else
	{
		printf("connect server successful!\n");
	}
 
	//要监视的描述符集合
	fd_set fds;
	
	//3 send msg
	while(1)
	{
		FD_ZERO(&fds);                  //清空文件描述符集合
		
		FD_SET(0,&fds);                 //把标准输入设备加入到集合中 
		
		FD_SET(socket_fd,&fds);         //把网络通信文件描述符加入到集合中 
		
		ret = select(socket_fd+1,&fds,NULL,NULL,NULL);
		if(ret < 0)//错误
		{
			perror("select fail:");
			return -1;
		}
		else if(ret > 0) //有活跃的
		{
			//判断是否 标准输入设备活跃 假设是则发送数据
			if(FD_ISSET(0,&fds))
			{
				char buf[1024] = {0};
				scanf("%s",buf);
				write(socket_fd,buf,strlen(buf));
				if(strcmp(buf, "exit") == 0)
				{
					break;
				}
			}
 
			//判断是否有收到数据
			if(FD_ISSET(socket_fd,&fds))
			{
				char buf[1024]={0};
				read(socket_fd,buf,sizeof(buf));
				printf("receive msg:%s\n",buf);
				if(strcmp(buf, "exit") == 0 || strcmp(buf, "") == 0)
				{
					break;
				}
 
 
			}
 
 
		}
	
	}
 
	//4 关闭通信socket
	close(socket_fd);
 
	return 0;
}
 