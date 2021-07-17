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


MYSQL *conn;

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

pthread_t thid;
int S_ID,E_ID,S_FD;

#define OPEN_MAX 1024

int  denglu(XINXI *YY,int sfd);            //登陆

int zhuce(XINXI *YY,int sfd);        //注册

int zhaohui(XINXI *YY,int sfd);     //找回密码

int zhaohui_1(XINXI *YY,int sfd);     //找回密码

void liaotian(DENN *XX, LIAOT *XZ,int sfd);

int xuanzhe_1(DENN *XX, XINXI *YY, int sfd);

int xuanzhe_2(DENN *XX, XINXI *YY, int sfd);

int TongZ(XINXI *YY, int sfd);    //添加好友通知

void HY_get(XINXI *YY, int id, int sfd);         //epoll收发信息

void HY_send(XINXI *YY, int sfd);                       //将未读信息发送给客户端

int set_1(int id);

int drop_1(int id);

int panduan_1(int id);                  //判断客户是否空闲

int panduan_2(int id);

void TX(XINXI *YY);

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

    tep.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP;      
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
            else if(ep[i].events & EPOLLIN)
            {
                sfd = ep[i].data.fd;
                //n = read(sfd, buf, sizeof(buf));         //读取套接字内容
                n = recv(sfd, &YY, sizeof(XINXI), 0);
                //printf("%d\n", n);
                //TX(&YY);
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
                    if(YY.ice_1 == 1)          //登陆，注册，找回密码
                    {
                        int ret;
                        
                        ret = 0;
                        if(YY.ice_2 == 11)       //登陆
                        {
                            //printf("id = %d\tice = %d\n", XX->id, XX->ice);
                            ret = denglu(&YY, sfd);
                        }
                        else if(YY.ice_2 == 12)      //注册
                        {
                            ret = zhuce(&YY, sfd);
                        }
                        else if(YY.ice_2 == 13)      //找回密码
                        { 
                            if(YY.ice_3 == 131)
                            {
                                zhaohui_1(&YY, sfd);
                            }
                            else
                            {
                                ret = zhaohui(&YY, sfd);
                            }
                        }
                        
                        if(ret == 1)
                        {
                        memset(XX, 0, sizeof(DENN));
                        XX->id = YY.m_id;              //将m_id存入XX中
                        sprintf(A, "update student set fd = %d where id = %d", sfd, XX->id);
                        mysql_query(conn,A);           //登陆后将fd存入数据库中
                        //获取name
                        MYSQL_RES *res_ptr;
                        MYSQL_ROW  res_row;
                        sprintf(A, "select name from student where id = %d", YY.m_id);
                        mysql_query(conn,A);
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        //printf("%s\n", res_row[0]);
                        strncpy(XX->name, res_row[0], sizeof(XX->name));   
                        mysql_free_result(res_ptr);
                        //获取yhlb
                        sprintf(A, "select hylb from student where id = %d", YY.m_id);
                        mysql_query(conn,A);
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        strncpy(XX->hylb, res_row[0], sizeof(XX->hylb));
                        mysql_free_result(res_ptr);
                        memset(&YY, 0, sizeof(XINXI));
                        }
                    }
                    else if(YY.ice_1 == 2)             //好友管理
                    {
                        xuanzhe_1(XX, &YY, sfd);
                    }
                    else if(YY.ice_1 == 3)             //聊天群管理
                    {

                    }
                    else if(YY.ice_1 == 4 && YY.ice_4 == 0)       //好友聊天
                    {
                        xuanzhe_2(XX, &YY, sfd);
                    }
                    else if(YY.ice_1 == 5)       //群聊天
                    {

                    }
                    else if(YY.ice_1 == 6)
                    {
                        YY.m_id = XX->id;
                        TongZ(&YY, sfd);
                    }
                    else if(YY.ice_4 == 666 && YY.ice_1 == 4)
                    {
                        //TX(&YY);
                        HY_send(&YY, sfd);
                    }
                    else if(YY.ice_4 == 777 && YY.ice_1 == 4)
                    {
                        //TX(&YY);
                        //printf("XX->id = %d\n", XX->id);
                        HY_get(&YY, YY.m_id, sfd);
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
    int ret;
    char A[100], *name = (char *) malloc(sizeof(char)*20);
    //获取name
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    sprintf(A, "select name from student where id = %d", id);
    ret = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    if(res_row != NULL)
    {
        strncpy(name, res_row[0], sizeof(name));   
    }
    else
    {
        printf("NULL\n");
    }
    mysql_free_result(res_ptr);

    return name;
}

char *getjl_from_id(int id_I, int id_II)
{
    //printf("id_I = %d, id_II = %d\n", id_I, id_II);
    char A[20],B[20], *jl = (char *)malloc(sizeof(char)*20);
    strncpy(A, getname_from_id(id_I), sizeof(A));
    strncpy(B, getname_from_id(id_II), sizeof(B));
    //printf("A: %s\n", A);
    //printf("B: %s\n", B);
    if(strcmp(A,B) > 0)
    {
        strncpy(jl, A, sizeof(A));
        strcat(jl,B);
    }
    else
    {
        strncpy(jl, B, sizeof(B));
        strcat(jl,A);
    }

    //printf("jl = %s\n", jl);

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
    char A[100],*beizhu = (char *) malloc(sizeof(char)*20);
    int ret;
    sprintf(A, "select beizhu from %shylb where id = %d", getname_from_id(m_id), id);
    ret = mysql_query(conn,A);
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res_ptr = mysql_store_result(conn);
    res_row = mysql_fetch_row(res_ptr);
    mysql_free_result(res_ptr);
    //printf("YYYYYYYYYY\n");
    if(res_row != NULL)
    {
        strncpy(beizhu, res_row[0], sizeof(beizhu));
    }
    //printf("XXXXXXXXXXXX\n");
    return beizhu;
}


int TongZ(XINXI *YY, int sfd)
{
    char A[100],B[50], ch;
    int ret, field, sum = 0;
    //printf("id = %d\n", YY->id);
    printf("m_id = %d\n", YY->m_id);
    sprintf(A, "select message from box where end_id = %d", YY->m_id);
    ret = mysql_query(conn,A);
    if(ret)
    {
        printf("wrong!\n");
    }
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    res_ptr = mysql_store_result(conn);
    field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
    //printf("field = %d\n", field);
    while(res_row=mysql_fetch_row(res_ptr))    
    {
        sum++;
        printf("sum1 = %d\n", sum);
    }
    //printf("sum2 = %d\n", sum);
    write(sfd, &sum, sizeof(int));
    mysql_free_result(res_ptr);

    do{

        read(sfd, B, sizeof(B));           //接收客户端选择
        if(strcmp(B, "A") == 0)
        {
            sprintf(A, "select message from box where end_id = %d", YY->m_id);
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
            sprintf(A, "select * from box where end_id = %d", YY->m_id);
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
                //printf("buf = %s\n",XZ->buf);
                if(strcmp(XZ->buf, "Y") == 0)                           //群人分开
                {
                    //人  
                    int ret_1,ret_2,ret_3;
                    //printf("s_id = %d\te_id = %d\n",s_id,e_id);
                    sprintf(A, "insert into %shylb (id, beizhu, jl, zt) values (%d, '%s', '%s', 0)", getname_from_id(e_id),s_id,XZ->beizhu,getjl_from_id(s_id,e_id));
                    ret_1 = mysql_query(conn,A);        //添加到好友列表
                    sprintf(A, "insert into %shylb (id, beizhu, jl, zt) values (%d, '%s', '%s', 0)", getname_from_id(s_id),e_id,res_row[field-1],getjl_from_id(s_id,e_id));
                    ret_2 = mysql_query(conn,A);        //添加到好友列表
                    sprintf(A, "create table %s (from_id int, xinxi varchar(100), end_id int, zt int)", getjl_from_id(s_id,e_id));
                    ret_3 = mysql_query(conn,A);        //创建信息表
                    sprintf(A, "delete from box where from_id = %d and end_id = %d", s_id, e_id);
                    ret = mysql_query(conn,A);        //删除该通知
                    if(ret == 0 && ret_1 == 0 && ret_2 == 0 && ret_3 == 0)
                        sprintf(B, "添加成功!\n");
                    else
                        sprintf(B, "添加失败!\n");
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

int ADD(XINXI *YY, DENN *XX)        //添加好友
{
    int field, fd, ret;

    if(YY->y_id == XX->id)
    {
        //printf("YYY\n");
        ret = -2;
        return ret;
    }

    char B[50];
    sprintf(B, "用户%s请求添加你为好友！", XX->name);
    char A[100];
    //printf("id = %d\n", XZ->id); 
    sprintf(A,"select * from %shylb where id = %d", getname_from_id(XX->id), YY->y_id);
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

    sprintf(A, "insert into box (from_id,end_id,message,ZT,ice,beizhu) values (%d,%d,'%s',0,0,'%s')", XX->id, YY->y_id, B, YY->beizhu);
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

  
int xuanzhe_1(DENN *XX, XINXI *YY, int sfd)
{   
    int ret;
    char buf[50],A[100];
    LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
    

        if(YY->ice_2 == 21)           //好友的添加，删除，查询
        {
            char B[50];
            if(YY->ice_3 == 211)                //添加好友
            {
                //printf("sfd = %d\n", sfd);
                ret = ADD(YY, XX);
                if(ret == 1)
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
                else if(ret == -2)
                {
                    sprintf(B, "该ID为当前帐号！\n");
                }
                write(sfd, B, sizeof(B));
            }
            else if(YY->ice_3 == 212)
            {
                    if(YY->zt == 1)   //根据ID删除好友
                    {
                        int ret_1,ret_2,ret_3;
                        sprintf(A, "delete from %shylb where id = %d", getname_from_id(XX->id), YY->y_id);
                        ret_1 = mysql_query(conn,A);
                        sprintf(A, "delete from %shylb where id = %d", getname_from_id(YY->y_id), XX->id);    
                        ret_2 = mysql_query(conn,A);
                        sprintf(A, "drop table %s", getjl_from_id(XX->id, YY->y_id));
                        ret_3 = mysql_query(conn,A);
                        if(ret_1 == 0 && ret_2 == 0 && ret_3 == 0) 
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
                    else if(YY->zt == 2)   //根据备注删除好友
                    {
                        int ID = getid_from_beizhu(XX->id, YY->beizhu);
                        YY->y_id = ID;
                        sprintf(A, "delete from %shylb where id = %d", getname_from_id(XX->id), YY->y_id);
                        ret = mysql_query(conn,A);
                        sprintf(A, "delete from %shylb where id = %d", getname_from_id(YY->y_id), XX->id);
                        if(ret)
                            ret = mysql_query(conn,A);
                        sprintf(A, "drop table %s", getjl_from_id(XX->id, YY->y_id));
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
            }
            else if(YY->ice_3 == 213)
            {
                    if(YY->zt == 1) //根据ID查询备注
                    {
                        sprintf(A, "select beizhu from %shylb where id = %d", getname_from_id(XX->id), YY->y_id);
                        ret = mysql_query(conn,A);
                        MYSQL_RES *res_ptr;
                        MYSQL_ROW  res_row;
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        if(res_row != NULL)
                        {
                            sprintf(buf, "ID为%d的好友备注为：%s\n", YY->y_id, res_row[0]);
                            write(sfd,buf, sizeof(buf));
                        }
                        mysql_free_result(res_ptr);
                    }
                    else if(YY->zt == 2) //根据备注查询ID
                    {
                        sprintf(A, "select id from %shylb where beizhu = '%s'", getname_from_id(XX->id), YY->beizhu);
                        ret = mysql_query(conn,A);
                        MYSQL_RES *res_ptr;
                        MYSQL_ROW  res_row;
                        res_ptr = mysql_store_result(conn);
                        res_row = mysql_fetch_row(res_ptr);
                        if(res_row != NULL)
                        {
                            sprintf(buf,"备注为%s的好友ID为：%d\n", YY->beizhu, res_row[0]);
                            write(sfd,buf, sizeof(buf));
                        }
                        mysql_free_result(res_ptr);
                    }
            }
        }
        else if(YY->ice_2 == 22)          //查看好友列表
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
        else if(YY->ice_2 == 23)             //查看好友状态
        {
            int field;
            char A[100];
            MYSQL_RES *res_ptr;
            MYSQL_ROW  res_row;
            sprintf(A, "select zt from student where id = %d", YY->y_id);
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
        else if(YY->ice_2 == 24)             //查看聊天记录
        {
            //printf("AAAAAAAAAAAAAAAAAA\n");
            int field;
            char A[100];
            MYSQL_RES *res_ptr;
            MYSQL_ROW  res_row;
            //printf("XX->id = %d\tYY->y_id = %d\n",XX->id,YY->y_id);
            sprintf(A, "select * from %s", getjl_from_id(XX->id, YY->y_id));
            int res = mysql_query(conn,A);
            if(res)
            {
                printf("wrong!!!\n");
            }
            res_ptr = mysql_store_result(conn);
            field = mysql_num_fields(res_ptr);      //返回你这张表有多少列
            //printf("field = %d\n", field);
            while(res_row=mysql_fetch_row(res_ptr))
            {
                for(int i = 0;i<field;i++)
                {
                    if(i == 0)
                    {
                        //printf("11111111111\n");
                        XZ->id = atoi(res_row[i]);
                        //printf("2222222222222\n");
                        strncpy(XZ->beizhu, getbeizhu_from_id(XX->id,XZ->id),sizeof(XZ->beizhu));
                        //printf("33333333333\n");
                    }
                    if(i == 1)
                    {
                        //printf("444444444444\n");
                        strncpy(XZ->xinxi, res_row[i], sizeof(XZ->xinxi));
                        //printf("5555555555555\n");
                    }
                }
                send(sfd, XZ, sizeof(LIAOT), 0);
            }
            //printf("over\n");
            sprintf(XZ->xinxi, "over");
            send(sfd, XZ, sizeof(LIAOT), 0);
            mysql_free_result(res_ptr);
        }
        else if(YY->ice_2 == 25)              //屏蔽好友信息
        {
            int field;
            sprintf(A, "select * from %shylb where id = %d",getname_from_id(XX->id), YY->y_id);
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
                sprintf(A, "update %shylb set zt = -1 where id = %d", getname_from_id(XX->id), YY->y_id);
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
    
    return 0;
}

void TJ_haoyou(int id,int sfd)       //获取未浏览信息数量
{

}

void HY_send(XINXI *YY,int sfd)                       //将未读信息发送给客户端
{
    //printf("BBBA\n");
    int ret,sum = 0;
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    LIAOT *XZ = (LIAOT*)malloc(sizeof(LIAOT));
    char A[100],B[50],beizhu[20];
    strncpy(YY->jl, getjl_from_id(YY->m_id, YY->y_id), sizeof(YY->jl));
    //printf("YY->jl = %s\tYY->m_id = %d\n",YY->jl,YY->m_id);
    sprintf(A, "select xinxi from %s where end_id = %d and zt = 1", YY->jl, YY->m_id);
    ret = mysql_query(conn,A);
    res_ptr = mysql_store_result(conn);
    while(res_row = mysql_fetch_row(res_ptr))
    {
        sum++;
    }
    XZ->zt = sum;    
    send(sfd, XZ, sizeof(LIAOT), 0);
    if(sum > 0)
    {
        mysql_free_result(res_ptr);
        ret = mysql_query(conn,A);
        res_ptr = mysql_store_result(conn);
        while(res_row = mysql_fetch_row(res_ptr))
        {
            //printf("res_row: %s\n", res_row[0]);
            strncpy(XZ->beizhu, getbeizhu_from_id(YY->m_id,YY->y_id), sizeof(XZ->beizhu));
            strncpy(XZ->xinxi, res_row[0], sizeof(XZ->xinxi));
            send(sfd, XZ, sizeof(LIAOT), 0);
            //sprintf(A, "update %s set zt = 0 where zt = 1 and end_id = %d and xinxi = %s", YY->jl, YY->m_id, res_row[0]);
            //ret = mysql_query(conn,A);      //将未读更新为已读
            //printf("ret = %d", ret);
            //printf("%s : %s\n", XZ->beizhu, XZ->xinxi);
        }
        mysql_free_result(res_ptr);
    }
    if(sum > 0)
    {
        sprintf(A, "update %s set zt = 0 where zt = 1", YY->jl);
        ret = mysql_query(conn,A);      //将未读更新为已读
        printf("ret = %d", ret);
    }
    free(XZ);
    //printf("AAA\n");
}

void HY_get(XINXI *YY, int id, int sfd)
{
    //printf("CCCCCCCCCCCcCCCCC\n");
    char A[100];//B[50],buf[50],beizhu[20];
    int ret;//,field,zt;   
    //TX(YY);
    //printf("YY->buf: %s\n", YY->buf);
    if(strcmp(YY->buf, "exit") == 0) 
    {
        //pthread_join(thid, NULL);
        return;
    }
    //printf("\n\n***************************************************\n");
    //printf("y_id = %d\n",YY->y_id);
    //printf("id = %d\n", id);
    //printf("buf = %s\n", YY->buf);
    sprintf(A, "insert into %s (from_id, xinxi, end_id, zt) values (%d, '%s', %d, 1)",getjl_from_id(id,YY->y_id), id, YY->buf, YY->y_id);
    //printf("%s\n",A);
    
    ret = mysql_query(conn,A);
    //printf("ret = %d\n\n\n", ret);
    //printf("DDDDDDDDDDDDDDDDDDDDDDDD\n");
}

void *thread(void *arg)
{
    while(1)
    {
        //HY_send(S_ID, E_ID,S_FD);
    }
}

int xuanzhe_2(DENN *XX, XINXI *YY, int sfd)
{
    int id, ret;
    char buf[50], A[100];
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    //获取未浏览信息数量
    TJ_haoyou(XX->id, sfd);
    YY->m_id = XX->id;

    //ret = recv(sfd, XZ, sizeof(LIAOT), 0);
        if(YY->ice_2 == 31)
        {
            //判断是否存在该好友
            //printf("name: %s\tYY->y_id = %d\n", getname_from_id(XX->id), YY->y_id);
            strncpy(YY->name, getname_from_id(XX->id), sizeof(YY->name));
            sprintf(A, "select * from %shylb where id = %d", YY->name, YY->y_id);
            ret = mysql_query(conn,A);
            //printf("rett = %d\n", ret);
            if(!ret)
            {
                res_ptr = mysql_store_result(conn);
                res_row = mysql_fetch_row(res_ptr);
                if(res_row != NULL)         //存在该好友
                {
                    mysql_free_result(res_ptr);
                    //printf("YY->m_id = %d\n", YY->m_id);
                    //printf("YY->y_id = %d\n", YY->y_id);
                    strncpy(YY->jl, getjl_from_id(YY->m_id, YY->y_id), sizeof(YY->jl));
                    strncpy(YY->beizhu, getbeizhu_from_id(YY->m_id, YY->y_id), sizeof(YY->beizhu));

                    sprintf(buf, "OK");
                    write(sfd, buf, sizeof(buf));
                    sleep(1);
                    //HY_send(YY,sfd);     //将未读信息发送个客户端
                    S_FD = sfd;
                    S_ID = YY->y_id;
                    E_ID = XX->id;
                    //pthread_create(&thid, NULL, thread, NULL);
                    return 0;                 //回到epoll，让epoll来随时接受信息
                }
                else if(res_row == NULL)                       //不存在该好友
                {
                    sprintf(buf, "不存在该好友!\n");
                    write(sfd, buf, sizeof(buf));
                }
                mysql_free_result(res_ptr);
            }
        }
}

int  denglu(XINXI *YY,int sfd)            //登陆
{
    int row, res, ret;
    char A[100],B[50];
    //printf("ID = %d\n", XX->id);
    sprintf(A, "select id from student where id = %d", YY->m_id);
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
        sprintf(A, "select id from student where id = %d and password = %s", YY->m_id, YY->password);
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
            sprintf(A, "update student set zt=1 where id = %d", YY->m_id);        //在线状态
            mysql_query(conn, A);
            strncpy(B,"登陆成功",50);
            ret = 1;
        }
        mysql_free_result(res_ptr);
    }
    write(sfd, B, sizeof(B));

    return ret;
}

int zhuce(XINXI *YY,int sfd)        //注册
{
    int res, ret;
    char A[100],B[50];
    sprintf(A, "select id from student where id = %d", YY->m_id);
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
        sprintf(A, "select id from student where id = %d and name = %s", YY->m_id, YY->name);
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
            strncpy(YY->hylb, YY->name, sizeof(YY->hylb));
            sprintf(A, "insert into student (id,password,name,qu,an,hylb,zt,sj) values (%d, '%s', '%s', '%s', '%s', '%s', 0, 0)",YY->m_id, YY->password,YY->name,YY->qu,YY->an, YY->hylb);
            int res = mysql_query(conn,A);
            strncpy(B,"注册成功",50);
            ret = 1;
            sprintf(A, "create table %s (id int, beizhu varchar(20), jl varchar(20), zt int)", YY->hylb);
            mysql_query(conn,A);           //创建好友列表
        }  
    }
    //printf("A\n");
    write(sfd, B, sizeof(B));

    return ret;
}

int zhaohui(XINXI *YY,int sfd)           //找回密码
{
    int res, field, ret;
    char A[200],B[50],C[100];
    sprintf(A, "select qu from student where id = %d", YY->m_id);
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

    return 1;
}

int zhaohui_1(XINXI *YY,int sfd)           //找回密码
{
    int ret,field;
    char A[100],B[50];
    MYSQL_RES *res_ptr;
    MYSQL_ROW  res_row;
    sprintf(A,"select an from student where id = %d AND an = '%s'",YY->m_id, YY->an);
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
        sprintf(A, "select password from student where an = '%s'", YY->an);
        ret = mysql_query(conn,A);
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

void TX(XINXI *YY)
{
    printf("ice_1 = %d\n", YY->ice_1);
    //printf("ice_2 = %d\n", YY->ice_2);
    printf("ice_4 = %d\n", YY->ice_4);
    printf("m_id  = %d\n", YY->m_id);
    printf("y_id  = %d\n", YY->y_id);
    //printf("password = %s\n", YY->password);
    printf("buf = %s\n", YY->buf);
}