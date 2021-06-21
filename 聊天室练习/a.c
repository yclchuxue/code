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

typedef struct xinxi{
	int ice;              //
	int id;
	char buf[1024];
}XINXI;

typedef struct denn{
	int ice;
	int id;
    int zt;
	char name[20];
	char password[16];
    char qu[200];
    char an[100];
    char yhlb[20];
}DENN;

typedef struct liaot{
	int ice;
	int id;
    int zt;
    char beizhu[20];
    char xinxi[200];
}LIAOT;

#define OPEN_MAX 1024

int  denglu(DENN *XX,int sfd);            //登陆

int zhuce(DENN *XX,int sfd);        //注册

int zhaohui(DENN *XX,int sfd);     //找回密码

void liaotian(DENN *XX, LIAOT *XZ,int sfd);

int xuznzhe(DENN *XX, LIAOT *XZ, int sfd);


int main()
{
    char A[100];
    DENN *XX = (DENN*)malloc(sizeof(DENN));
    LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
    XINXI YY;

    conn = mysql_init(conn);    //初始化一个句柄
    mysql_library_init(0,NULL,NULL);   //初始化数据库

    if(conn == NULL)
    {
        printf("mysql_init failed!!!\n");
        exit(1);
    }
    conn = mysql_real_connect(conn,"127.0.0.1","root","181219","db1",0,NULL,0);  //连接数据库
    mysql_set_character_set(conn,"utf8");      //调整为中文字符

    if(conn == NULL)
    {
        printf("mysql_real_connect failed!!!\n");
        exit(1);
    }

    sprintf(A, "update student set zt=0");      //服务器重启，所有用户都不在线
    mysql_query(conn,A);

	int cfd, lfd, sfd, ret, n, i, maxi, efd, nready, PD;
	struct sockaddr_in caddr, saddr;
	char buf[50], str[INET_ADDRSTRLEN];           //缓存区
	socklen_t clen;

	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(9999);               //端口号9999
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);      //TCP

	lfd = socket(AF_INET, SOCK_STREAM, 0);            //创建套接字
    int mw_optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (char *)&mw_optval,sizeof(mw_optval));
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
                n = recv(sfd, &YY, sizeof(YY), 0);
                //printf("%s\n",buf);
                //n = recv(sfd, XX, sizeof(DENN),0);
                //printf("%d\n",XX->ice);
                //printf("id = %d,password = %s\n",XX->id,XX->password);//无限循环
                if(n == 0)         //客户端关闭
                {
                    ret = epoll_ctl(efd, EPOLL_CTL_DEL, sfd, NULL);    //将套接字sfd从等待队列中删除
                    
                    sprintf(A, "update student set zt = 0 where id = %d", XX->id);
                    mysql_query(conn,A);        //用户退出，将状态设置为不在线

                    close(sfd);        //关闭该套接字
                }
                else
                {
                    if(YY.ice == 1)          //登陆，注册，找回密码
                    {
                        recv(sfd, XX, sizeof(DENN),0);
                        int i = XX->ice, ret;
                        do
                        {
                            ret = 0;
                            if(i == 1)       //登陆
                            {
                                ret = denglu(XX, sfd);
                                if(ret == 1)
                                {
                                    break;
                                }
                                else
                                {
                                    recv(sfd, XX, sizeof(DENN),0);
                                    i = XX->ice;
                                }
                            }
                            if(i == 2)      //注册
                            {
                                ret = zhuce(XX, sfd);
                                recv(sfd, XX, sizeof(DENN),0);
                                i = XX->ice;
                            }
                            if(i == 3)      //找回密码
                            { 
                                ret = zhaohui(XX, sfd);
                                recv(sfd, XX, sizeof(DENN),0);
                                i = XX->ice;
                            }
                        }while(1);
                    
                        //获取name
                        MYSQL_RES *res_ptr;
                        MYSQL_ROW  res_row;
                        sprintf(A, "select name from student where id = %d", XX->id);
                        mysql_query(conn,A);
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        //printf("%s\n", res_row[0]);
                        strncpy(XX->name, res_row[0], sizeof(XX->name));   
                        mysql_free_result(res_ptr);
                        //获取yhlb
                        sprintf(A, "select yhlb from student where id = %d", XX->id);
                        mysql_query(conn,A);
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        strncpy(XX->yhlb, res_row[0], sizeof(XX->yhlb));
                        mysql_free_result(res_ptr);

                        //LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
                        //recv(sfd, XZ, sizeof(LIAOT), 0);       //接受到客户端信息，判断客户端需要的功能
                        //liaotian(XX, XZ, sfd);
                    }
                    else if(YY.ice == 2)
                    {
                        recv(sfd, XZ, sizeof(LIAOT), 0);
                        xuanzhe(XX, XZ, sfd);
                    }
                }
            }
        }
    }
    close(lfd);
    return 0;
}

int xuznzhe(DENN *XX, LIAOT *XZ, int sfd)
{
    if(XZ->ice == 1)
    {

    }
    else if(XZ->ice == 2)          //查看好友列表
    {
        int field;
        char B[50], A[100];
        sprintf(A, "select * from %s", XX->yhlb);
        int res = mysql_query(conn,A);
        if(res)
        {
            printf("wrong!!!\n");
        }
        MYSQL_RES *res_ptr;
        MYSQL_ROW  res_row;
        res_ptr = mysql_store_result(conn);
        field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
        while(res_row=mysql_fetch_row(res_ptr) )
	    {
		    for(int i=0;i<field;i++)
    		{
	    		strcat(B, res_row[i]);
                strcat(B, "\t");
		    }
    		write(sfd, B, sizeof(B));
            memset(B, 0, sizeof(B));
	    }
        sprintf(B, "over");
        write(sfd, B, sizeof(B));
        mysql_free_result(res_ptr);
    }
    else if(XZ->ice == 3)             //查看好友状态
    {
        int field;
        char A[100];
        MYSQL_RES *res_ptr;
        MYSQL_ROW  res_row;
        sprintf(A, "select zt from student where id = %d", XZ->id);
        int res = mysql_query(conn,A);
        if(res)
        {
            printf("wrong!!!\n");
        }
        res_ptr = mysql_store_result(conn);
        field = mysql_num_fields(res_ptr);
        XZ->zt = res_row[0];
        send(sfd, XZ, sizeof(LIAOT), 0);
        mysql_free_result(res_ptr);
    }
    else if(XZ->ice == 4)             //查看聊天记录
    {
        int field;
        char A[100];
        MYSQL_RES *res_ptr;
        MYSQL_ROW  res_row;
        sprintf(A, "select * from %s", XX->yhlb);             //此处需要更改，×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××8
        int res = mysql_query(conn,A);
        if(res)
        {
            printf("wrong!!!\n");
        }
        res_ptr = mysql_store_result(conn);
        field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
        
    }
    else if(XZ->ice == 5)
    {

    }
    
    
}

int  denglu(DENN *XX,int sfd)            //登陆
{
    int row, res, ret;
    char A[100],B[50];
    sprintf(A, "select id from student where id = %d", XX->id);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    mysql_free_result(res_ptr);
    if(res_row == NULL)
    {
        //id错误
        strncpy(B,"无此id",50);
        ret = 0;
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
            ret = 0;
        }
        else
        {
            sprintf(A, "update student set zt=1 where id = %d", XX->id);        //在线状态
            mysql_query(conn, A);
            strncpy(B,"登陆成功",50);
            ret = 1;
        }
        mysql_free_result(res_ptr);
    }
    write(sfd, B, sizeof(B));

    return ret;
}

int zhuce(DENN *XX,int sfd)        //注册
{
    int res, ret;
    char A[100],B[50];
    sprintf(A, "select id from student where id = %d", XX->id);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    mysql_free_result(res_ptr);
    if(res_row)
    {
        //id已经存在
        ret = 0;
        strncpy(B,"id已经被注册，请更换一个id",50);
    }
    else
    {
        sprintf(A, "insert into student (id,password,name,qu,an,yhlb,zt) values (%d, '%s', '%s', '%s', '%s', '%s', 0)",XX->id, XX->password,XX->name,XX->qu,XX->an, XX->yhlb);
        int res = mysql_query(conn,A);
        strncpy(B,"注册成功",50);
        ret = 1;

        sprintf(A, "create table %s (id int, beizhu varchar(20), jl varchar(20))", XX->yhlb);
        mysql_query(conn,A);           //创建好友列表
    }
    //printf("A\n");
    write(sfd, B, sizeof(B));

    return ret;
}

int zhaohui(DENN *XX,int sfd)
{
    int res, field, ret;
    char A[200],B[50],C[100];
    sprintf(A, "select qu from student where id = %d", XX->id);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
    //printf("AA%dAA\n", field);
    //printf("%s\n",res_row[0]);
    strncpy(A,res_row[0],200);
    write(sfd, A, sizeof(A));               //将密保问题发送到客户端
    mysql_free_result(res_ptr);
    recv(sfd, XX, sizeof(DENN),0);          //接收到客户端的答案
    sprintf(A,"select an from student where id = %d AND an = %s",XX->id, XX->an);
    //printf("%d\t%s\n",XX->id,XX->an);
    mysql_query(conn, A);
    res_ptr = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res_ptr);
    mysql_free_result(res_ptr);

    if(row == NULL)
    {
        strncpy(B,"答案错误",50);
        ret = 0;
    }
    else
    {
        sprintf(A, "select password from student where an = %s", XX->an);
        res = mysql_query(conn,A);
        res_ptr = mysql_store_result(conn);
        res_row = mysql_fetch_row(res_ptr);
        field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
        sprintf(B,"密码为：%s",res_row[0]);
        mysql_free_result(res_ptr);
        ret = 1;
    }
    write(sfd, B, sizeof(B));        //将密码发送到客户端

    return ret;
}


void liaotian(DENN *XX, LIAOT *XZ,int sfd)
{
    char A[100], B[50], BUF[1024];
    if(XZ->ice == 0)       //查看所有好友
    {
        int field;
        sprintf(A, "select * from %s", XX->yhlb);
        int res = mysql_query(conn,A);
        if(res)
        {
            printf("wrong!!!\n");
        }
        MYSQL_RES *res_ptr;
        MYSQL_ROW  res_row;
        res_ptr = mysql_store_result(conn);
        field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
        while(res_row=mysql_fetch_row(res_ptr) )
	    {
		    for(int i=0;i<field;i++)
    		{
	    		strcat(B, res_row[i]);
                strcat(B, "\t");
		    }
    		write(sfd, B, sizeof(B));
            memset(B, 0, sizeof(B));
	    }
        sprintf(B, "over");
        write(sfd, B, sizeof(B));
        mysql_free_result(res_ptr);
    }
    else if(XZ->ice == 1)      //添加好友
    {
        //printf("%s\n", XX->name);
        sprintf(A,"insert into %s (id, beizhu, jl) values (%d, '%s', 'LTJL%d')", XX->name, XZ->id, XZ->beizhu, XZ->id);
        //printf("%s\n",A);
        int res = mysql_query(conn,A);
        if(res)
        {
            sprintf(B, "添加失败!");
        }
        else
        {
            sprintf(B, "添加成功！");
        }
        write(sfd, B, sizeof(B));
        
    }

}