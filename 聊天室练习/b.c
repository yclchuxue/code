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
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;

typedef struct xinxi{
	int ice_1;              //功能选择
	int ice_2;
	int ice_3;
	int ice_4;
	int m_id;               //客户id
	int y_id;               //好友id
	int zt;                //状态
	char name[20];         //name
	char password[16];     //密码
	char qu[200];          //问题
	char an[100];          //答案
	char beizhu[20];      //备注	
	char hylb[20];         //好友列表
	char jl[20];           //聊天记录
	char buf[200];       //信息内容
}XINXI;

typedef struct denn{
	int ice;               //功能选择
	int id;                //id
	int zt;                //状态
	char name[20];         //name
	char password[16];     //密码
	char qu[200];          //问题
	char an[100];          //答案
	char hylb[20];         //好友列表
}DENN;

typedef struct liaot{
	int ice;
	int id;
	int zt;
	char buf[50];
	char beizhu[20];
    char xinxi[200];
}LIAOT;

int Socket_fd, Y_ID, M_ID,N;

void C_denn(XINXI *YY, int socket_fd);

void C_zhuce(XINXI *YY, int socket_fd);

void C_zhaohui(XINXI *YY, int socket_fd);

void C_haoy(XINXI *YY, DENN *XX, int socket_fd);

void C_haoyouliaot(XINXI *YY, DENN *XX, int socket_fd);

int C_TongZ(XINXI *YY, DENN *XX, int socket_fd);

void C_get(int socket_fd);

void *thread(void *arg);

void face(XINXI *YY);

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

	Socket_fd = socket_fd;

	do{          //*******************************************************************************

		YY.ice_1 = 1;
		do
		{
			memset(&YY, 0, sizeof(XINXI));
			YY.ice_1 = 1;
			face(&YY);     //1        2       3          0
			if(YY.ice_2 == 11)             //登陆
			{
				C_denn(&YY, socket_fd);
				M_ID = YY.m_id;
				break;
			}
			else if(YY.ice_2 == 12)        //注册
			{
				C_zhuce(&YY, socket_fd);
				//face(XX);
			} 
			else if(YY.ice_2 == 13)       //找回密码
			{
				C_zhaohui(&YY, socket_fd);

				//face(XX);
			}
			else if(YY.ice_2 == 0)
			{
				exit(0);
			}
			else
			{
				printf("无此功能，请重新选择！\n");
			}
		}while(1);

		do{                                                  //************************************************
			memset(&YY, 0, sizeof(XINXI));
			int ice;
			printf("*************************************\n");
			printf("***********1 好友管理******************\n");
			printf("***********2 聊天群管理****************\n");
			printf("***********3 好友聊天*****************\n");
			printf("***********4 群聊天*******************\n");
			printf("***********5 查看通知******************\n");
			printf("***********0 退出帐号 *****************\n");
			printf("请输入你的选择：");
			scanf("%d", &ice);

			if(ice == 1)
			{
				YY.ice_1 = 2;
				//printf("%d\n", YY.ice);
				//int ret = send(socket_fd, &YY, sizeof(YY), 0);
				C_haoy(&YY, XX, socket_fd);
			}
			else if(ice == 2)
			{

			}
			else if(ice == 3)
			{
				YY.ice_1 = 4;
				//int ret  = send(socket_fd, &YY, sizeof(YY),0);
				C_haoyouliaot(&YY, XX, socket_fd);
			}
			else if(ice == 4)
			{

			}
			else if(ice == 5)
			{
				YY.ice_1 = 6;
				//printf("%d\n", YY.ice);
				int ret = send(socket_fd, &YY, sizeof(YY), 0);
				C_TongZ(&YY, XX, socket_fd);
			}
			else if(ice == 0)
			{
				break;
			}

		}while(1);

	}while(1);

	//4 关闭通信socket
	close(socket_fd);
 
	do{

	}while(1);
	
	return 0;
}

int C_TongZ(XINXI *YY, DENN *XX, int socket_fd)
{
	int sum, ret;
	char buf[50];
	read(socket_fd, &sum, sizeof(int));

	do{

	printf("你一共有%d条通知！！！\n", sum);
	printf("[A] 查看所有通知  [B] 逐个处理所有通知 [R] 退出\n");
	printf("请选择功能:");
	scanf("%s", buf);
	if (strcmp(buf, "A") == 0)
	{
		write(socket_fd, buf, sizeof(buf));
		do
		{
			read(socket_fd, buf, sizeof(buf));
			if(strcmp(buf, "over") != 0)
			{
				printf("%s\n", buf);
			}
		} while (strcmp(buf, "over") != 0);
	}
	else if (strcmp(buf, "B") == 0)
	{
		write(socket_fd, buf, sizeof(buf));
		do
		{
			read(socket_fd, buf, sizeof(buf));
			if(strcmp(buf, "over") != 0)
			{
				LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
				char ch, B[50],beizhu[20];
				printf("%s\n", buf);
				printf("[Y] 同意  [N] 忽略\n");
				printf("请输入你的选择:");
				scanf("%s", &B);
				//write(socket_fd, ch, sizeof(char));
				if(strcmp(B, "Y") == 0)
				{
					printf("请输入备注:");
					scanf("%s", beizhu);
					strncpy(XZ->buf, B, sizeof(B));
					strncpy(XZ->beizhu, beizhu, sizeof(beizhu));
					send(socket_fd, XZ, sizeof(LIAOT), 0);
					read(socket_fd,buf, sizeof(buf));
					printf("%s\n",buf);
				}
				else if(strcmp(B, "N") == 0)
				{
					send(socket_fd, XZ, sizeof(LIAOT), 0);
				}
			}
		} while (strcmp(buf, "over") != 0);
		return 0;
	}
	else if (strcmp(buf, "R") == 0)
	{
		write(socket_fd, buf, sizeof(buf));
		return 0;
	}
	else
	{
		printf("无此选项请重新输入！\n");
	}
	
	}while(1);

}

void C_haoy(XINXI *YY, DENN *XX, int socket_fd)
{
	int id,ret;
	char beizhu[20];
	char buf[50];
	do
	{
		//printf("%d\n", ret);
		int ic;
		LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
		memset(XZ, 0, sizeof(LIAOT));
		printf("**********************************************\n");
		printf("******************0 退出***********************\n");
		printf("**************1 好友的添加，删除，查询************\n");
		printf("******************2 查看好友列表****************\n");
		printf("*****************3 查看好友状态*****************\n");
		printf("******************4 查看聊天记录****************\n");
		printf("****************5 屏蔽好友消息******************\n");
		printf("请输入要选择的功能：");
		scanf("%d", &ic);
		
		if(ic == 1)      //添加,删除，查询好友
		{
			YY->ice_2 = 21;
			//send(socket_fd, XZ, sizeof(LIAOT), 0);
			char buf[50];
			char ch;
			printf("A 添加好友\n");
			printf("B 删除好友\n");
			printf("C 查询好友\n");
			printf("请输入你的选择:");
			scanf("%s", buf);
			//int ret = write(socket_fd,buf, sizeof(buf));
			//strncpy(XZ->buf, buf, sizeof(XZ->buf));
			//printf("%d\n", ret);
			if(strcmp(buf, "A") == 0)
        	{
				YY->ice_3 = 211;
				printf("请输入要添加好友的ID：");
				scanf("%d",&id);
				YY->y_id = id;
				printf("请输入备注：");
				scanf("%s",&beizhu);
				strncpy(YY->beizhu, beizhu, sizeof(beizhu));
				//printf("size = %d\nid = %d\n", sizeof(LIAOT), XZ->id);
				ret = send(socket_fd, YY, sizeof(XINXI), 0);  //将添加好友信息发送
				//printf("%d\n", ret);
				ret = read(socket_fd,buf, sizeof(buf));
				//printf("%d\n", ret);
				printf("%s",buf);
        	}
    	    else if(strcmp(buf, "B") == 0)
        	{
				YY->ice_3 = 212;
				do
				{
					int i;
					printf("************************************\n");
					printf("**********0 退出 ********************\n");
					printf("**********1 根据ID 删除 **************\n");
					printf("**********2 根据备注删除 **************\n");
					printf("请输入你的选择：");
					scanf("%d", &i);		
					YY->zt = i;
					if(i == 0)
					{
						//send(socket_fd, XZ, sizeof(LIAOT), 0);
						break;
					}
					else if(i == 1)
					{
						printf("请输入你要删除的好友的ID：");
						scanf("%d",&id);
						YY->y_id = id;
						ret = send(socket_fd, YY, sizeof(XINXI),0);
						ret = read(socket_fd, buf, sizeof(buf));
						printf("%s",buf);
					}
					else if(i == 2)
					{
						printf("请输入你要删除好友的备注：");
						scanf("%s",beizhu);
						strncpy(XZ->beizhu,beizhu,sizeof(beizhu));
						ret = send(socket_fd, YY, sizeof(XINXI),0);
						ret = read(socket_fd, buf, sizeof(buf));
						printf("%s",buf);
					}
				}while(1);
    	    }
    	    else if(strcmp(buf, "C") == 0)
    	    {
				YY->ice_3 = 213;
				do
				{
					int i;
					printf("************************************\n");
					printf("**********0 退出 ********************\n");
					printf("**********1 根据ID查询备注 **************\n");
					printf("**********2 根据备注查询ID **************\n");
					printf("请输入你的选择：");
					scanf("%d", &i);
					YY->zt = i;
					if(i == 0)
					{
						//ret = send(socket_fd, XZ, sizeof(LIAOT), 0);
						break;
					}		
					else if(i == 1)
					{
						printf("请输入好友的ID：");
						scanf("%d", &id);
						YY->y_id = id;
						ret = send(socket_fd, YY, sizeof(XINXI),0);
						ret = read(socket_fd, buf, sizeof(buf));
						printf("%s",buf);
					}
					else if(i == 2)
					{
						printf("请输入好友备注：");
						scanf("%s", beizhu);
						strncpy(YY->beizhu, beizhu, sizeof(beizhu));
						ret = send(socket_fd, YY, sizeof(XINXI), 0);
						ret = read(socket_fd, buf, sizeof(buf));
						printf("%s",buf);
					}
				} while (1);
				

    	    }
			/*
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
			*/
		}
		else if (ic == 2)      //查看好友列表
		{
			YY->ice_2 = 22;
			send(socket_fd, YY, sizeof(XINXI),0);
			do
			{
				read(socket_fd,buf, sizeof(buf));
				if(strcmp(buf,"over") != 0)
				{
					printf("%s\n",buf);
				}
			} while (strcmp(buf, "over") != 0);
		}
		else if(ic == 3)        //查看好友状态
		{
			YY->ice_2 = 23;
			printf("请输入要查询好友的ID：");
			scanf("%d", &id);
			YY->y_id = id;
			send(socket_fd, YY, sizeof(XINXI), 0);
			recv(socket_fd, XZ, sizeof(LIAOT), 0);
			if(XZ->zt)
			{
				printf("该好友在线！\n");
			}
			else if(XZ->zt == 0)
			{
				printf("该好友不在线!\n");
			}
			else if(XZ->zt == -1)
			{
				printf("你无此ID的好友！\n");
			}
		}
		else if(ic == 4)        //查看聊天记录
		{
			YY->ice_2 = 24;
			printf("请输入你要查询好友的ID：");
			scanf("%d", &id);
			YY->y_id = id;
			send(socket_fd, YY, sizeof(XINXI), 0);
			do
			{
				recv(socket_fd,XZ, sizeof(LIAOT), 0);
				if(strcmp(XZ->xinxi, "over") != 0)
				{
					if(XZ->id == id)      //从好友处发来的信息
					{
						printf("%s : %s\n", XZ->beizhu, XZ->xinxi);
					}
					else
					{
						printf("MINE ：%s\n", XZ->xinxi);
					}
				}
			} while (strcmp(XZ->xinxi, "over") != 0);
			//printf("end\n");
		}
		else if(ic == 5)        //屏蔽好友消息
		{
			YY->ice_2 = 25;
			printf("请输入你要屏蔽的好友ID:");
			scanf("%d",&id);
			YY->y_id = id;
			send(socket_fd, YY, sizeof(XINXI),0);
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
		else if(ic == 0)
		{
			YY->ice_2 = 20; 
			return ;     //退出循环
		}
		else
		{
			printf("无此功能请重新选择!!!!!!!!!!!!\n");
		}
	} while (1);
} 

void C_denn(XINXI *YY, int socket_fd)
{
	int I;
	char P[16],buf[50];
	do
	{
		printf("ID:");
		scanf("%d",&I);
		printf("Password:");
		scanf("%s",P);
		YY->m_id = I;
		YY->zt = 1;
		strncpy(YY->password, P, 16);
		//printf("id = %d,password = %s\n",XX->id,XX->password);
		int ret = send(socket_fd, YY, sizeof(XINXI),0);
		ret = read(socket_fd,buf, sizeof(buf));
		if(ret)
		{
			printf("%s\n",buf);
		}
	}while(strcmp(buf,"登陆成功") != 0);
}

void C_get(int socket_fd)
{
	int n;
	LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
	XINXI *YY = (XINXI*)malloc(sizeof(XINXI));
		//pthread_mutex_lock(&lock);              //加锁
		YY->ice_4 = 666;
		YY->ice_1 = 4;
		YY->m_id = M_ID;
		//printf("mid = %d\tyid = %d\n",M_ID,YY->y_id);
		YY->y_id = Y_ID;
		send(Socket_fd, YY, sizeof(XINXI), 0);
		//printf("JJJJJJJJJ\n");
		recv(Socket_fd, XZ, sizeof(LIAOT), 0);
		//printf("SSSSSSSSSSSS\n");
		n = XZ->zt;
		//printf("XZ->zt = %d\n", XZ->zt);
		if(XZ->zt > 0)
		{
			for(int i = 0;i<n;i++)
			{
				printf("\b\b\b\b\b\b\n");
				char buf[1024]={0};
				//read(socket_fd,buf,sizeof(buf));
				recv(Socket_fd, XZ, sizeof(LIAOT), 0);
				printf("%s : %s\n", XZ->beizhu, XZ->xinxi);
				//printf("MINE: ");
			}
		}
	free(YY);
	free(XZ);
	//printf("HHHHHHHHHHHHHHH\n");
	return ;
}

void *thread(void *arg)
{
	int n;
	LIAOT XZ;
	XINXI YY;
	do
	{
		sleep(3);
		//pthread_mutex_lock(&lock);              //加锁
		YY.ice_4 = 666;
		YY.ice_1 = 4;
		YY.m_id = M_ID;
		//printf("mid = %d\tyid = %d\n",M_ID,YY->y_id);
		YY.y_id = Y_ID;
		send(Socket_fd, &YY, sizeof(XINXI), 0);
		//printf("JJJJJJJJJ\n");
		recv(Socket_fd, &XZ, sizeof(LIAOT), 0);
		//printf("SSSSSSSSSSSS\n");
		n = XZ.zt;
		//printf("XZ->zt = %d\n", XZ->zt);
		if(XZ.zt > 0)
		{
			for(int i = 0;i<n;i++)
			{
				printf("\b\b\b\b\b\b\n");
				char buf[1024]={0};
				//read(socket_fd,buf,sizeof(buf));
				recv(Socket_fd, &XZ, sizeof(LIAOT), 0);
				printf("%s : %s\n", XZ.beizhu, XZ.xinxi);
			}
		}
	} while (1);
	//printf("HHHHHHHHHHHHHHH\n");
	//return ;

}

void C_haoyouliaot(XINXI *YY, DENN *XX, int socket_fd)
{
	pthread_t thid;
	int id,ret,ic,N = 0;
	char beizhu[20],buf[50];

		LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
		printf("**************************************\n");
		printf("*************0 退出 *******************\n");
		printf("*************1 与好友聊天 **************\n");
		printf("请输入你的选择：");
		scanf("%d", &ic);
		if(ic == 0)
		{
			return;
		}
		else if(ic == 1)
		{
			YY->ice_2 = 31;
			YY->ice_4 = 0;
			printf("请输入好友的ID:");
			scanf("%d",&id);
			YY->y_id = id;
			send(socket_fd,YY, sizeof(XINXI), 0);
			read(socket_fd,buf, sizeof(buf));
			if(strcmp(buf, "OK") != 0)
			{
				printf("%s",buf);
			}
			//fflush(stdin);
			Y_ID = id;
			YY->m_id = M_ID;
			
			//pthread_mutex_init(&lock, NULL);                   //初始化锁
    		//pthread_cond_init(&cond, NULL);                    //初始化条件变量

			if(pthread_create(&thid, NULL, thread, NULL) != 0)
			{
				//创建失败
				printf("创建线程失败！\n");
				return ;
			}
			else
			{
				printf("创建成功！\n");
			}

			write(STDOUT_FILENO, "MINE :", 7);

			do{

				//pthread_mutex_lock(&lock);              //加锁
				
				
    			//pthread_cond_wait(&cond, &lock);        //将锁释放（其他地方可以抢到锁），并睡眠等待唤醒      
				/*
				printf("BBBBB\n");
				i++;
				printf("i = %d\n", i);
				if(i == 5)
				{
					i = 0;
					printf("YYYYYYYYYYYYYYYY\n");
					C_get(socket_fd);
				}
				*/
				//要监视的描述符集合
				fd_set fds;
				FD_ZERO(&fds);                  //清空文件描述符集合
		
				FD_SET(0,&fds);                 //把标准输入设备加入到集合中 
		
				//FD_SET(socket_fd,&fds);         //把网络通信文件描述符加入到集合中 


				ret = select(socket_fd+1,&fds,NULL,NULL,NULL);
		 		if(ret < 0)//错误
				{
					perror("select fail:");
					return ;
				}
				else if(ret > 0) //有活跃的
				{
					//判断是否 标准输入设备活跃 假设是则发送数据
					if(FD_ISSET(0,&fds))
					{
						N = 1;
						char buf[200] = {0};
						scanf("%s",buf);
						strncpy(YY->buf, buf, sizeof(YY->buf));
						YY->ice_4 = 777;
						//printf("buf: %s\n", YY->buf);
						//printf("ice_1 = %d\n",YY->ice_1);
						ret = send(socket_fd, YY, sizeof(XINXI), 0);

						if(strcmp(buf, "exit") == 0)
						{
							pthread_join(thid, NULL);   //销毁线程
							break;
						}
						printf("\nMINE: ");
						N = 0;
					}
				}
				//pthread_mutex_unlock(&lock);
			}while(1);
		}

}

void C_zhuce(XINXI *YY, int socket_fd)
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
		YY->m_id = I;
		sprintf(hylb, "%shylb", name);
		strncpy(YY->password, P, sizeof(P));
		strncpy(YY->name, name, sizeof(name));
		strncpy(YY->qu, qu, sizeof(qu));
		strncpy(YY->an, an, sizeof(an));
		strncpy(YY->hylb, hylb, sizeof(hylb));          //好友列表，以name为mysql—table名
		send(socket_fd, YY, sizeof(XINXI),0);
		read(socket_fd,buf, sizeof(buf));
		printf("%s\n",buf);
	}while (strcmp(buf,"注册成功") != 0);
}

void C_zhaohui(XINXI *YY, int socket_fd)
{
	int I;
	char an[100],qu[200],buf[50];
	do
	{
		printf("请输入你的id：");
		scanf("%d",&I);
		YY->m_id = I;
		send(socket_fd, YY, sizeof(XINXI),0);      //将ID发送到服务器
		read(socket_fd, qu, sizeof(qu));          //读取密保问题
		printf("%s\n", qu);
		scanf("%s",an);                           //回答问题
		strncpy(YY->an, an, sizeof(an));
		YY->ice_3 = 131;
		send(socket_fd, YY, sizeof(XINXI),0);       //将答案发送到服务端
		read(socket_fd, buf, sizeof(buf));
		printf("%s\n", buf);
	}while(strcmp(buf, "答案错误") == 0);
}

void face(XINXI *YY)
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
	YY->ice_2 = 10+n;         //选择功能
}