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

typedef struct xinxi{
	int ice;              //功能选择
	int id;               //客户id
	char buf[1024];       //信息内容
}XINXI;

typedef struct denn{
	int ice;               //功能选择
	int id;                //id
	int zt;                //状态
	char name[20];         //name
	char password[16];     //密码
	char qu[200];          //问题
	char an[100];          //答案
	char yhlb[20];         //好友列表
}DENN;

typedef struct liaot{
	int ice;
	int id;
	int zt;
	char beizhu[20];
    char xinxi[200];
}LIAOT;

void C_denn(DENN *XX, int socket_fd);

void C_zhuce(DENN *XX, int socket_fd);

void C_zhaohui(DENN *XX, int socket_fd);

void face(DENN *XX);

int main()
{
	int id;
	DENN *XX = (DENN*)malloc(sizeof(DENN));
	XINXI YY;
	char buf[50];
	//face(XX);

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


	do
	{
		YY.ice = 1;
		send(socket_fd, &YY, sizeof(YY), 0);
		face(XX);
		if(XX->ice == 1)             //登陆
		{
			C_denn(XX, socket_fd);
			break;
		}
		else if(XX->ice == 2)        //注册
		{
			C_zhuce(XX, socket_fd);
			//face(XX);
		} 
		else if(XX->ice == 3)       //找回密码
		{
			C_zhaohui(XX, socket_fd);
		
			//face(XX);
		}
	}while(1);

	
	do
	{
		YY.ice = 2;
		send(socket_fd, &YY, sizeof(YY), 0);
		int ic;
		LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
		printf("**********************************************\n");
		printf("******************0 退出***********************\n");
		printf("**************1 好友的添加，删除，查询************\n");
		printf("******************2 查看好友列表****************\n");
		printf("*****************3 查看好友状态*****************\n");
		printf("******************4 查看聊天记录****************\n");
		printf("****************5 屏蔽好友消息******************\n");
		printf("请输入要选择的功能：");
		scanf("%d", &ic);
		XZ->ice = ic;    //选择的功能
		
		if(ic == 1)      //添加,删除，查询好友
		{
			int I;
			char B[20];
			printf("请输入好友ID:");
			scanf("%d", &I);
			printf("请输入备注:");
			scanf("%s", B);
			XZ->id = I;               //好友ID
			XZ->ice = ic;
			strncpy(XZ->beizhu, B, sizeof(B));          //备注名
			send(socket_fd, XZ, sizeof(LIAOT),0);

			read(socket_fd, buf, sizeof(buf));
			printf("%s\n",buf);
		}
		else if (ic == 2)      //查看好友列表
		{
			XZ->ice = ic;
			send(socket_fd, XZ, sizeof(LIAOT),0);
			do
			{
				read(socket_fd,buf, sizeof(buf));
				printf("%s\n",buf);

			} while (strcmp(buf, "over") != 0);
		}
		else if(ic == 3)        //查看好友状态
		{
			printf("请输入要查询好友的ID：");
			scanf("%d", &id);
			XZ->id = id;
			XZ->ice = ic;
			send(socket_fd, XZ, sizeof(LIAOT), 0);
			recv(socket_fd, XZ, sizeof(LIAOT), 0);
			if(XZ->zt)
			{
				printf("该好友在线！\n");
			}
			else
			{
				printf("该好友不在线!\n");
			}
			if(XZ->zt == -1)
			{
				printf("你无此ID的好友！\n");
			}
		}
		else if(ic == 4)        //查看聊天记录
		{
			printf("请输入你要查询好友的ID：");
			scanf("%d", &id);
			XZ->id = id;
			XZ->ice = ic;
			send(socket_fd, XX, sizeof(DENN),0);
			do
			{
				recv(socket_fd,XZ, sizeof(LIAOT), 0);
				if(XZ->id == id)
				{
					printf("%s: %s\n", XZ->beizhu, XZ->xinxi);
				}
				else
				{
					printf("自己：%s\n", XZ->xinxi);
				}
			} while (strcmp(XZ->xinxi, "over") != 0);
		}
		else if(ic == 5)        //屏蔽好友消息
		{
			printf("请输入你要屏蔽的好友ID:");
			scanf("%d",&id);
			XZ->id = id;
			XZ->ice = ic;
			send(socket_fd, XX, sizeof(DENN),0);
			recv(socket_fd, XZ, sizeof(LIAOT), 0);
			if(XZ->zt == -1)
			{
				printf("你无此ID的好友！\n");
			}
			else if(XZ->zt == 0)
			{
				printf("屏蔽失败！\n");
			}
			else
			{
				printf("屏蔽成功!\n");
			}
		}
		else
		{
			printf("无此功能请重新选择!!!!!!!!!!!!\n");
		}
	} while (1);
	

	//4 关闭通信socket
	close(socket_fd);
 
	do{

	}while(1);
	
	return 0;
}
 

 void C_denn(DENN *XX, int socket_fd)
{
	int I;
	char P[16],buf[50];
	do
	{
		printf("ID:");
		scanf("%d",&I);
		printf("Password:");
		scanf("%s",P);
		XX->id = I;
		XX->zt = 1;
		strncpy(XX->password, P, 16);
		//printf("id = %d,password = %s\n",XX->id,XX->password);
		send(socket_fd, XX, sizeof(DENN),0);
		read(socket_fd,buf, sizeof(buf));
		printf("%s\n",buf);
	}while(strcmp(buf,"登陆成功") != 0);
}

void C_zhuce(DENN *XX, int socket_fd)
{
	int I;
	char P[16],buf[50],name[20],qu[200],an[100],hylb[20];
	do
	{
		printf("ID:");
		scanf("%d",&I);
		printf("Password:");
		scanf("%s",P);
		printf("Name: ");
		scanf("%s",name);
		printf("请设置密保问题：");
		scanf("%s",qu);
		printf("请输入答案：");
		scanf("%s",an);
		XX->id = I;
		sprintf(hylb, "%shylb", name);
		strncpy(XX->password, P, sizeof(P));
		strncpy(XX->name, name, sizeof(name));
		strncpy(XX->qu, qu, sizeof(qu));
		strncpy(XX->an, an, sizeof(an));
		strncpy(XX->yhlb, hylb, sizeof(hylb));          //好友列表，以name为mysql—table名
		send(socket_fd, XX, sizeof(DENN),0);
		read(socket_fd,buf, sizeof(buf));
		printf("%s\n",buf);
	}while (strcmp(buf,"注册成功") != 0);
}

void C_zhaohui(DENN *XX, int socket_fd)
{
	int I;
	char an[100],qu[200],buf[50];
	do
	{
		printf("请输入你的id：");
		scanf("%d",&I);
		XX->id = I;
		send(socket_fd, XX, sizeof(DENN),0);      //将ID发送到服务器
		read(socket_fd, qu, sizeof(qu));          //读取密保问题
		printf("%s\n", qu);
		scanf("%s",an);                           //回答问题
		strncpy(XX->an, an, sizeof(an));
		send(socket_fd, XX, sizeof(DENN),0);       //将答案发送到服务端
		read(socket_fd, buf, sizeof(buf));
		printf("%s\n", buf);
	}while(strcmp(buf, "答案错误") == 0);
}

void face(DENN *XX)
{
	int n;
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
}