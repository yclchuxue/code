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
#include <pthread.h>
#include <assert.h>

/* 线程池里所有运行和等待的任务都是一个CThread_worker
由于所有任务都在链表里，所以是一个链表结构 */
typedef struct worker
{
    /*回调函数，任务运行时会调用此函数，注意也可声明成其它形式*/
    void *(*process) (void *arg);
    void *arg;/*回调函数的参数*/
    struct worker *next;
} CThread_worker;

/*线程池结构*/
typedef struct
{
    pthread_mutex_t queue_lock;     //锁
    pthread_cond_t queue_ready;     //条件变量

    /*链表结构，线程池中所有等待任务*/
    CThread_worker *queue_head;

    /*是否销毁线程池*/
    int shutdown;

    /* 线程数组 */
    pthread_t *threadid;

    /*线程池中允许的活动线程数目*/
    int max_thread_num;

    /*当前等待队列的任务数目*/
    int cur_queue_size;
} CThread_pool;

MYSQL *conn;

typedef struct xinxi{
	int ice;              //功能选择
	int id;               //id
    char beizhu[20];      //备注
	char buf[1024];       //信息传递
}XINXI;

typedef struct denn{
	int ice;               //功能选择
	int id;                //ID
    int zt;                //状态
	char name[20];         //网名
	char password[16];     //密码
    char qu[200];          //密保问题
    char an[100];          //答案      
    char hylb[20];         //好友列表
}DENN;

typedef struct liaot{
	int ice;                //功能选择
	int id;                 //id
    int zt;                 //状态
    char buf[50];                //功能选择
    char beizhu[20];        //备注
    char xinxi[200];        //信息
}LIAOT;

#define OPEN_MAX 1024

int  denglu(DENN *XX,int sfd);            //登陆

int zhuce(DENN *XX,int sfd);        //注册

int zhaohui(DENN *XX,int sfd);     //找回密码

void liaotian(DENN *XX, LIAOT *XZ,int sfd);

int xuanzhe_1(DENN *XX, LIAOT *XZ, int sfd);

int xuanzhe_2(DENN *XX, LIAOT *XZ, int sfd);

int TongZ(XINXI *YY, int sfd);    //添加好友通知

void HY_get(XINXI *YY, int id, int sfd);         //epoll收发信息

void HY_send(int s_id,int e_id, int sfd);                       //将未读信息发送给客户端

int set_1(int id);

int drop_1(int id);

int panduan_1(int id);                  //判断客户是否空闲

int panduan_2(int id);

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
                    sprintf(A, "update student set fd = 0 where id = %d",XX->id);
                    mysql_query(conn,A);        //用户退出后将fd归0

                    close(sfd);        //关闭该套接字
                }
                else
                {
                    //printf("YY->ice = %d\n", YY.ice);
                    if(YY.ice == 1)          //登陆，注册，找回密码
                    {
                        recv(sfd, XX, sizeof(DENN),0);
                        int ret;
                        do
                        {
                            ret = 0;
                            if(XX->ice == 1)       //登陆
                            {
                                //printf("id = %d\tice = %d\n", XX->id, XX->ice);
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
                            else if(XX->ice == 2)      //注册
                            {
                                ret = zhuce(XX, sfd);
                            }
                            else if(XX->ice == 3)      //找回密码
                            { 
                                ret = zhaohui(XX, sfd);
                            }
                            memset(XX, 0, sizeof(DENN));
                            recv(sfd, XX, sizeof(DENN),0);
                        }while(1);
                        sprintf(A, "update student set fd = %d where id = %d", sfd, XX->id);
                        mysql_query(conn,A);           //登陆后将fd存入数据库中


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
                        sprintf(A, "select hylb from student where id = %d", XX->id);
                        mysql_query(conn,A);
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        strncpy(XX->hylb, res_row[0], sizeof(XX->hylb));
                        mysql_free_result(res_ptr);

                        //LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
                        //recv(sfd, XZ, sizeof(LIAOT), 0);       //接受到客户端信息，判断客户端需要的功能
                        //liaotian(XX, XZ, sfd);

                        //printf("AAA\n");
                        memset(&YY, 0, sizeof(XINXI));
                    }
                    else if(YY.ice == 2)             //好友管理
                    {
                        //printf("BBB\n");
                        //recv(sfd, XZ, sizeof(LIAOT), 0);
                        xuanzhe_1(XX, XZ, sfd);
                    }
                    else if(YY.ice == 3)             //聊天群管理
                    {
                        if(panduan_1(YY.id) == 0)
                        {
                            set_1(YY.id);
                            //pool_add_worker(myprocess_2, &sfd);       //myprocess为测试函数，及工作函数
                            drop_1(YY.id);
                        }
                        else            //将请求写入消息盒子
                        {

                        }
                    }
                    else if(YY.ice == 4)       //好友聊天
                    {
                        xuanzhe_2(XX, XZ, sfd);
                    }
                    else if(YY.ice == 5)       //群聊天
                    {

                    }
                    else if(YY.ice == 6)
                    {
                        TongZ(&YY, sfd);
                    }
                    else if(YY.ice == 666)
                    {
                        HY_send(YY.id,XX->id,sfd);
                    }
                    else if(YY.ice == 777)
                    {
                        HY_get(&YY, XX->id, sfd);
                    }
                }
            }
        }
    }

    //printf("BBB\n");

    close(lfd);
    return 0;
}

char *getname_from_id(int id)
{
    char A[100], *name = (char *) malloc(sizeof(char)*20);
    //获取name
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    sprintf(A, "select name from student where id = %d", id);
    mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    strncpy(name, res_row[0], sizeof(name));   
    mysql_free_result(res_ptr);

    return name;
}

char *getjl_from_id(int id_I, int id_II)
{
    char A[20],B[20], *jl = (char *)malloc(sizeof(char)*20);
    strncpy(A, getname_from_id(id_I), sizeof(A));
    strncpy(B, getname_from_id(id_II), sizeof(B));
    if(strcmp(A,B) > 0)
    {
        strncpy(jl, A, sizeof(A));
        strcat(jl,B);
    }

    //printf("%s", jl);

    return jl;
}

int getid_from_beizhu(int id,char *beizhu)
{
    char A[100];
    int ID;
    int ret;
    sprintf(A, "select id from %shylb where beizhu = '%s'", getname_from_id(id), beizhu);
    ret = mysql_query(conn,A);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    mysql_free_result(res_ptr);
    if(res_row != NULL)
    {
        ID = atoi(res_row[0]);
        return ID;
    }
    else
    {
        return 0;
    }
}

char *getbeizhu_from_id(int m_id,int id)
{
    char A[100];//beizhu[20];
    int ret;
    sprintf(A, "select beizhu from %shylb where id = %d", getname_from_id(m_id), id);
    ret = mysql_query(conn,A);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    mysql_free_result(res_ptr);
    if(res_row != NULL)
    {
        return res_row[0];
    }
}


int TongZ(XINXI *YY, int sfd)
{
    char A[100],B[50], ch;
    int ret, field, sum = 0;
    //printf("id = %d\n", YY->id);
    sprintf(A, "select message from box where end_id = %d", YY->id);
    ret = mysql_query(conn,A);
    if(ret)
    {
        printf("wrong!\n");
    }
    else
    {
        
    }

    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res_ptr = mysql_store_result(conn);
    field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
    printf("field = %d\n", field);
    while(res_row=mysql_fetch_row(res_ptr))    
    {
        sum++;
    }
    write(sfd, &sum, sizeof(int));
    mysql_free_result(res_ptr);

    do{

        read(sfd, B, sizeof(B));           //接收客户端选择
        if(strcmp(B, "A") == 0)
        {
            sprintf(A, "select message from box where end_id = %d", YY->id);
            ret = mysql_query(conn,A);
            MYSQL_RES *res_ptr;
            MYSQL_ROW  res_row;
            res_ptr = mysql_store_result(conn);
            field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
            //printf("field = %d\n", field);
            while(res_row=mysql_fetch_row(res_ptr)) 
            {
                sprintf(B, "%s", res_row[0]);
                write(sfd, B, sizeof(B));
            }
            sprintf(B, "over");
            write(sfd, B, sizeof(B));
            mysql_free_result(res_ptr);
        }
        else if(strcmp(B, "B") == 0)
        {
            sprintf(A, "select * from box where end_id = %d", YY->id);
            ret = mysql_query(conn,A);
            MYSQL_RES *res_ptr;
            MYSQL_ROW  res_row;
            res_ptr = mysql_store_result(conn);
            field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
            //printf("field = %d\n", field);
            while(res_row=mysql_fetch_row(res_ptr)) 
            {
                int i,s_id, e_id;
                for(i = 0; i < 2; i++)
                {
                    if(i == 0)
                    {
                        s_id = atoi(res_row[i]);
                    }
                    else if(i == 1)
                    {
                        e_id = atoi(res_row[i]);
                    }
                }
                LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
                sprintf(B, "%s", res_row[i]);
                write(sfd, B, sizeof(B));
                recv(sfd, XZ, sizeof(LIAOT), 0);
                if(strcmp(XZ->buf, "Y") == 0)                           //群人分开
                {
                    //人  
                    sprintf(A, "insert into %shylb (id, beizhu, jl, zt) values (%d, '%s', '%s', 0)", getname_from_id(e_id),s_id,XZ->beizhu,getjl_from_id(s_id,e_id));
                    mysql_query(conn,A);        //添加到好友列表
                    sprintf(A, "insert into %shylb (id, beizhu, jl, zt) values (%d, '%s', '%s', 0)", getname_from_id(s_id),e_id,res_row[field-1],getjl_from_id(s_id,e_id));
                    mysql_query(conn,A);        //添加到好友列表
                    sprintf(A, "create table %s (from_id int, xinxi varchar(100), end_id int, zt int)", getjl_from_id(s_id,e_id));
                    mysql_query(conn,A);        //创建信息表
                    sprintf(A, "delete from box where from_id = %d and end_id = %d", s_id, e_id);
                    mysql_query(conn,A);        //删除该通知

                    sprintf(B, "添加成功!\n");
                    write(sfd, B, sizeof(B));
                    //群
                }
                else if(strcmp(XZ->buf, "N") == 0)
                {
                    sprintf(A, "delete from box where from_id = %d and end_id = %d", s_id, e_id);
                    mysql_query(conn,A);        //删除该通知
                }   
            }
            sprintf(B, "over");
            write(sfd, B, sizeof(B));
            mysql_free_result(res_ptr);
            return 0;                      //结束循环
        }
        else if(strcmp(B, "R") == 0)
        {
            return 0;                     //结束循环
        }

    }while(1);

}

int set_1(int id)            //设置为忙碌
{
    char A[100];
    sprintf(A, "update student set sj = 1 where id = %d", id);
    int ret = mysql_query(conn,A);
    if(ret == 0)
    {
        return 0;
    }
    return 1;
}

int drop_1(int id)           //设置为空闲
{
    char A[100];
    sprintf(A, "update student set sj = 0 where id = %d", id);
    int ret = mysql_query(conn,A);
    if(ret == 0)
    {
        return 0;
    }
    return 1;
}

int panduan_1(int id)                  //判断客户是否空闲
{
    char A[100];
    int sj, res, field;
    //printf("HHH %d HHH\n", id);
    sprintf(A, "select sj from student where id = %d", id);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
    if(res_row == NULL)
    {
        printf("NULL\n");
    }
    
    sj = atoi(res_row[0]);

    return sj;
}

int panduan_2();

int ADD(LIAOT *XZ, DENN *XX)        //临时添加好友
{
    int field, fd, ret;
    char B[50];
    sprintf(B, "用户%s请求添加你为好友！", XX->name);
    char A[100];
    //printf("id = %d\n", XZ->id); 
    sprintf(A,"select * from %shylb where id = %d", getname_from_id(XX->id), XZ->id);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    ret = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    mysql_free_result(res_ptr);
    if(res_row != NULL)
    {
        ret = -1;            //该用户已经是好友
        return ret;
    }

    sprintf(A, "insert into box (from_id,end_id,message,ZT,ice,beizhu) values (%d,%d,'%s',0,0,'%s')", XX->id, XZ->id, B, XZ->beizhu);
    ret = mysql_query(conn,A);
    if(ret)
    {
        printf("wrong!\n");
        return 0;              //信息发送失败
    }
    else
    {
        return 1;              //信息发送成功
    }


    /*  
    sprintf(A, "select fd from student where id = %d", XZ->id);      //XZ->id出错

    int res = mysql_query(conn,A);
    if(res)
    {
        printf("wrong!!!\n");
    }
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res_ptr = mysql_store_result(conn);
    field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
    res_row=mysql_fetch_row(res_ptr);
    printf("B\n");
    if(res_row == NULL)
    {
        printf("over\n");
    }
    else
    {
        fd = atoi(res_row[0]);            //获取目标好友的套接字码
        printf("fd  = %d\n", fd);
    }
    */
}

  
int xuanzhe_1(DENN *XX, LIAOT *XZ, int sfd)
{   
    int ret;
    char buf[50],A[100];
    
    do{                                                      //****************************

        recv(sfd, XZ, sizeof(LIAOT), 0);
        if(XZ->ice == 1)           //好友的添加，删除，查询
        {
            char B[50];
            if(strcmp(XZ->buf, "A") == 0)                //添加好友
            {
                //printf("sfd = %d\n", sfd);
                ret = ADD(XZ, XX);
                if(ret)
                {
                    sprintf(B, "信息发送成功！\n");
                }
                else if(ret == 0)
                {
                    sprintf(B, "信息发送失败！\n");
                }
                else if(ret == -1)
                {
                    sprintf(B, "该用户已经是好友，无需再添加!\n");
                }
                write(sfd, B, sizeof(B));
            }
            else if(strcmp(buf, "B") == 0)
            {
                do
                {
                    if(XZ->zt == 0)        //结束循环
                    {
                        break;
                    }
                    else if(XZ->zt == 1)   //根据ID删除好友
                    {
                        sprintf(A, "delete from %shylb where id = %d", getname_from_id(XX->id), XZ->id);
                        ret = mysql_query(conn,A);
                        sprintf(A, "delete from %shylb where id = %d", getname_from_id(XZ->id), XX->id);
                        if(ret)
                            ret = mysql_query(conn,A);
                        sprintf(A, "drop table %s", getjl_from_id(XX->id, XZ->id));
                        if(ret)
                            ret = mysql_query(conn,A);

                        if(ret == 0)
                        {
                            sprintf(buf, "删除成功!\n");
                            write(sfd, buf, sizeof(buf));
                        }
                        else
                        {
                            sprintf(buf, "删除失败!\n");
                            write(sfd, buf, sizeof(buf));
                        }
                    }
                    else if(XZ->zt == 2)   //根据备注删除好友
                    {
                        int ID = getid_from_beizhu(XX->id, XZ->beizhu);
                        XZ->zt = 1;
                        XZ->id = ID;
                        continue;
                    }
                    recv(sfd, XZ, sizeof(LIAOT), 0);
                }while(1);
            }
            else if(strcmp(buf, "C") == 0)
            {
                do
                {
                    if(XZ->zt == 0)     //结束循环
                    {
                        break;
                    }
                    else if(XZ->zt == 1) //根据ID查询备注
                    {
                        sprintf(A, "select beizhu from %shylb where id = %d", getname_from_id(XX->id), XZ->id);
                        ret = mysql_query(conn,A);
                        MYSQL_RES *res_ptr;
                        MYSQL_ROW  res_row;
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        if(res_row != NULL)
                        {
                            sprintf(buf, "ID为%d的好友备注为：%s\n", XZ->id, res_row[0]);
                            write(sfd,buf, sizeof(buf));
                        }
                        mysql_free_result(res_ptr);
                    }
                    else if(XZ->zt == 2) //根据备注查询ID
                    {
                        sprintf(A, "select id from %shylb where beizhu = '%s'", getname_from_id(XX->id), XZ->beizhu);
                        ret = mysql_query(conn,A);
                        MYSQL_RES *res_ptr;
                        MYSQL_ROW  res_row;
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        if(res_row != NULL)
                        {
                            sprintf(buf,"备注为%s的好友ID为：%d\n", XZ->beizhu, res_row[0]);
                            write(sfd,buf, sizeof(buf));
                        }
                        mysql_free_result(res_ptr);
                    }
                    recv(sfd, XZ, sizeof(LIAOT),0);
                } while (1);   
            }
        }
        else if(XZ->ice == 2)          //查看好友列表
        {
            int field;
            char B[50], A[100];
            sprintf(A, "select * from %shylb", getname_from_id(XX->id));
            int res = mysql_query(conn,A);
            if(res)
            {
                printf("wrong!!!\n");
            }
            MYSQL_RES *res_ptr;
            MYSQL_ROW  res_row;
            res_ptr = mysql_store_result(conn);
            field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
            //printf("field = %d\n", field);
            memset(B, 0, sizeof(B));
            while(res_row=mysql_fetch_row(res_ptr) )
	        {
		        for(int i=0;i<2;i++)
    		    {
                    if(i != 2)
                    {
                        //printf("%s\n", res_row[i]);
    	    		    strncat(B, res_row[i],strlen(res_row[i]));
                        strcat(B, "\t");
                    }
                }
    		    write(sfd, B, sizeof(B));
                memset(B, 0, sizeof(B));
	        }
            //printf("CCC\n");
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
            res_row=mysql_fetch_row(res_ptr);
            if(res_row != NULL)
            {
                XZ->zt = atoi(res_row[0]);
            }
            mysql_free_result(res_ptr);
            sprintf(A, "select * from %shylb where id = %d", getname_from_id(XX->id), XZ->id);
            ret = mysql_query(conn,A);
            res_ptr = mysql_store_result(conn);
            res_row = mysql_fetch_row(res_ptr);
            if(res_row == NULL)
            {
                XZ->zt = -1;            //没有此ID的好友
            }
            mysql_free_result(res_ptr);
            
            send(sfd, XZ, sizeof(LIAOT), 0);
        }
        else if(XZ->ice == 4)             //查看聊天记录
        {
            int field;
            char A[100];
            MYSQL_RES *res_ptr;
            MYSQL_ROW  res_row;
            XINXI *YY = (XINXI*)malloc(sizeof(XINXI));
            sprintf(A, "select * from %s", getjl_from_id(XX->id, XZ->id));             //此处需要更改，×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××8
            int res = mysql_query(conn,A);
            if(res)
            {
                printf("wrong!!!\n");
            }
            res_ptr = mysql_store_result(conn);
            field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
            while(res_row=mysql_fetch_row(res_ptr))
            {
                for(int i = 0;i<field;i++)
                {
                    if(i == 0)
                        strncpy(YY->beizhu, getbeizhu_from_id(XX->id,atoi(res_row[i])),sizeof(YY->beizhu));
                    if(i == 1)
                        strncpy(YY->buf, res_row[i], sizeof(YY->buf));
                }
                send(sfd, YY, sizeof(XINXI), 0);
            }
            sprintf(YY->buf, "over");
            send(sfd, YY, sizeof(XINXI), 0);
            mysql_free_result(res_ptr);
            free(YY);
        }
        else if(XZ->ice == 5)              //屏蔽好友信息
        {
            int field;
            sprintf(A, "select * from %shylb where id = %d",getname_from_id(XX->id), XZ->id);
            MYSQL_RES *res_ptr;
            MYSQL_ROW  res_row;
            ret = mysql_query(conn,A);
            if(ret)
            {
                printf("wrong!!!\n");
            }
            res_ptr = mysql_store_result(conn);
            field = mysql_num_fields(res_ptr); 
            res_row=mysql_fetch_row(res_ptr);
            if(res_row != NULL)
            {
                sprintf(A, "update %shylb set zt = -1 where id = %d", getname_from_id(XX->id), XZ->id);
                ret = mysql_query(conn,A);
                sprintf(buf, "屏蔽成功!\n");
            }
            else
            {
                sprintf(buf, "你无此ID的好友!\n");
            }   
            write(sfd, buf, sizeof(buf));
            mysql_free_result(res_ptr);  
        }
        else if(XZ->ice == 0)
        {
            return 0;         //结束循环
        }

    }while(1);
    
    return 0;
}

void TJ_haoyou(int id,int sfd)       //获取未浏览信息数量
{

}

void HY_send(int s_id,int e_id,int sfd)                       //将未读信息发送给客户端
{
    int ret;
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    XINXI *YY = (XINXI*)malloc(sizeof(XINXI));
    char A[100],B[50],beizhu[20];
    sprintf(A, "select xinxi from %s where end_id = %d and zt = 1", getjl_from_id(s_id,e_id), e_id);
    ret = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    while(res_row = mysql_fetch_row(res_ptr))
    {
        strncpy(YY->beizhu, getbeizhu_from_id(e_id,s_id), sizeof(YY->beizhu));
        strncpy(YY->buf, res_row[0], sizeof(YY->buf));
        send(sfd, YY, sizeof(XINXI), 0);
    }

    free(YY);
}

void HY_get(XINXI *YY, int id, int sfd)
{
    char A[100];//B[50],buf[50],beizhu[20];
    int ret;//,field,zt;   
    sprintf(A, "insert into %s (from_id, xinxi, end_id, zt) values (%d, '%s', %d, 1)", getjl_from_id(id,YY->id), id, YY->buf, YY->id);
    ret = mysql_query(conn,A);
    if(ret)
    {

    }

    //mysql_free_result(res_ptr);
}

int xuanzhe_2(DENN *XX, LIAOT *XZ, int sfd)
{
    int id, ret;
    char buf[50], A[100];
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    //获取未浏览信息数量
    TJ_haoyou(XX->id, sfd);

    ret = recv(sfd, XZ, sizeof(LIAOT), 0);
    do
    {
        if(XZ->ice == 0)
        {
            break;
        }
        else if(XZ->ice == 1)
        {
            //判断是否存在该好友
            sprintf(A, "select * from %shylb where id = %d", getname_from_id(XX->id), XZ->id);
            ret = mysql_query(conn,A);
            if(!ret)
            {
                res_ptr = mysql_store_result(conn);
                res_row = mysql_fetch_row(res_ptr);
                if(res_row != NULL)         //存在该好友
                {
                    sprintf(buf, "OK");
                    write(sfd, buf, sizeof(buf));
                    HY_send(XZ->id, XX->id,sfd);
                    return 0;                 //回到epoll，让epoll来随时接受信息
                }
                else                        //不存在该好友
                {
                    sprintf(buf, "不存在该好友!\n");
                    write(sfd, buf, sizeof(buf));
                }
                mysql_free_result(res_ptr);
            }
        }
        recv(sfd, XZ, sizeof(LIAOT), 0);
    }while(1);
}

int  denglu(DENN *XX,int sfd)            //登陆
{
    int row, res, ret;
    char A[100],B[50];
    //printf("ID = %d\n", XX->id);
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
        sprintf(A, "select id from student where id = %d and name = %s", XX->id, XX->name);
        res = mysql_query(conn,A);
        res_ptr = mysql_store_result(conn);
        res_row = mysql_fetch_row(res_ptr);
        mysql_free_result(res_ptr);
        if(res_row)                      //name已存在
        {
            ret = 0;
            strncpy(B, "name已经被注册，请更换一个name", 50);
        }
        else
        {
            sprintf(A, "insert into student (id,password,name,qu,an,hylb,zt,sj) values (%d, '%s', '%s', '%s', '%s', '%s', 0, 0)",XX->id, XX->password,XX->name,XX->qu,XX->an, XX->hylb);
            int res = mysql_query(conn,A);
            strncpy(B,"注册成功",50);
            ret = 1;
            sprintf(A, "create table %s (id int, beizhu varchar(20), jl varchar(20), zt int)", XX->hylb);
            mysql_query(conn,A);           //创建好友列表
        }  
    }
    //printf("A\n");
    write(sfd, B, sizeof(B));

    return ret;
}

int zhaohui(DENN *XX,int sfd)           //找回密码
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
    sprintf(A,"select an from student where id = %d AND an = '%s'",XX->id, XX->an);
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
        sprintf(A, "select password from student where an = '%s'", XX->an);
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
