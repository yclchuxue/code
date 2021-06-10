/**********************************
 * 服务器端
 * *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <arpa/inet.h>

MYSQL *conn;
typedef struct denn{
	int ice;
	int id;
	char name[20];
	char password[16];
    char qu[200];
    char an[100];
}DENN;

#define OPEN_MAX 1024

void denglu(DENN *XX,int sfd)            //登陆
{
    int row, res;
    char A[100],B[50];
    sprintf(A, "select id from student where id = %d", XX->id);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);

    if(res_row == NULL)
    {
        //id错误
        strncpy(B,"无此id",50);
    }
    else
    {
        sprintf(A, "select id from student where id = %d and password = %s", XX->id, XX->password);
        res = mysql_query(conn,A);
        res_ptr = mysql_store_result(conn);
        res_row = mysql_fetch_row(res_ptr);                   
        if(res_row == NULL)
        {
        //password错误
            strncpy(B,"密码错误",50);
        }
        else
        {
            strncpy(B,"登陆成功",50);
        }
    }
    write(sfd, B, sizeof(B));
}

void zhuce(DENN *XX,int sfd)        //注册
{
    int res;
    char A[100],B[50];
    sprintf(A, "select id from student where id = %d", XX->id);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);

    if(res_row)
    {
        //id已经存在
        strncpy(B,"id已经被注册，请更换一个id",50);
    }
    else
    {
        //res_row = NULL;                       //重置res_row
        MYSQL_ROW row;
        sprintf(A, "select name from student where name = %s", XX->name);
        res = mysql_query(conn,A);
        res_ptr = mysql_store_result(conn);
        res_row = mysql_fetch_row(res_ptr);                   
        if(res_row == NULL)
        {
            //printf("%s\n",mysql_fetch_fields(res_ptr)[0].name);
            sprintf(A, "insert into student (id,password,name,qu,an) values (%d, '%s', '%s', '%s', '%s')",XX->id, XX->password,XX->name,XX->qu,XX->an);
            int res = mysql_query(conn,A);
            strncpy(B,"注册成功",50);
        }
        else
        {
            //name已经存在
            strncpy(B,"name已经被注册，请更换一个name",50);
        }
    }
    //printf("A\n");
    write(sfd, B, sizeof(B));
}

void zhaohui(DENN *XX,int sfd)
{

}

int main()
{
    DENN *XX = (DENN*)malloc(sizeof(DENN));

    conn = mysql_init(conn);    //初始化一个句柄

    if(conn == NULL)
    {
        printf("mysql_init failed!!!\n");
        exit(1);
    }

    conn = mysql_real_connect(conn,"127.0.0.1","root","181219","db1",0,NULL,0);  //连接数据库

    if(conn == NULL)
    {
        printf("mysql_real_connect failed!!!\n");
        exit(1);
    }

	int cfd, lfd, sfd, ret, n, i, maxi, efd, nready;
	struct sockaddr_in caddr, saddr;
	char buf[BUFSIZ], str[INET_ADDRSTRLEN];           //缓存区
	socklen_t clen;

	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(9999);               //端口号9999
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);      //TCP

	lfd = socket(AF_INET, SOCK_STREAM, 0);            //创建套接字
	bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));    //绑定端口
	listen(lfd, 128);                //设置监听套接字

    struct epoll_event tep, ep[OPEN_MAX];   //定义描述符和响应队列

    efd = epoll_create(OPEN_MAX);   

    tep.events = EPOLLIN;      
    tep.data.fd = lfd;

    ret = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &tep);     //将监听套接字加入等待队列

    int j = 0;
    while(1)
    {
        j++;
        nready = epoll_wait(efd, ep, OPEN_MAX, -1);     //等待就绪套接字，将就绪套接字放入响应队列，返回响应套接字的个数
        for(i = 0; i < nready; i++)
        {
            if(!ep[i].events & EPOLLIN)     //该套接字描述符不可读
                continue;
            if(ep[i].data.fd == lfd)          //监听套接字有响应，有新的套接字要连接服务器
            {
                clen = sizeof(caddr);
                cfd = accept(lfd, (struct sockaddr *)&caddr, &clen);        //连接套接字

                printf("connection--port:%d\tip:%s\n", ntohs(caddr.sin_port), inet_ntop(AF_INET, &caddr.sin_addr.s_addr, str, sizeof(str)));

                tep.data.fd = cfd;
                tep.events = EPOLLIN;
                ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &tep);      //将新套接字加入等待队列
            }
            else
            {
                sfd = ep[i].data.fd;
                //n = read(sfd, buf, sizeof(buf));         //读取套接字内容
                recv(sfd, XX, sizeof(DENN),0);
                //printf("%d\n",XX->ice);
                //printf("id = %d,password = %s\n",XX->id,XX->password);//无限循环
                if(n == 0)         //内容已经读完
                {
                    ret = epoll_ctl(efd, EPOLL_CTL_DEL, sfd, NULL);    //将套接字sfd从等待队列中删除
                    close(sfd);        //关闭该套接字
                }
                else
                {
                    switch(XX->ice)
                    {
                        case 1: 
                            {
                                denglu(XX, sfd);
                                break;
                            }
                        case 2:
                            {
                                zhuce(XX, sfd);
                                break;
                            }
                        case 3:
                            {
                                zhaohui(XX, sfd);
                                break;
                            }
                    }
                }
            }
        }
    }
    close(lfd);
    return 0;
}