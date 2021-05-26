/*********************************
 * 该程序中的函数在库my_recv.h中
 * ******************************/
#define MY_RECV_C

#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "my_recv.h"

#define BUFSIZE    1024

//自定义的错误处理函数
void my_err(const char *err_string, int line)
{
    fprintf(stderr, "line:%d", line);
    perror(err_string);
    exit(1);
}

/*********************************
 * 从套接字上读取一次数据（以\n结束）
 * 从conn_fd连接套接字上接收数据 
 * 读取到的数据保存到date_buf缓冲中
 * len为date_buf所指的空间长度
 * 错误返回-1,服务器已关闭连接返回0,成功返回读取的字节数
 * *****************************/
int my_recv(int conn_fd, char *date_buf, int len)
{
    static char recv_buf[BUFSIZE];           //自定义缓冲区，//BUFSIZE定义在my_recv.h中
    static char *pread;                      //指向下一次读取数据位置
    static int  len_remain = 0;              //自定义缓冲区中剩余字节数
    int         i;

    //如果缓冲区没有数据，则从套接字读取数据
    if(len_remain <= 0)
    {
        if((len_remain = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
        {
            my_err("recv", __LINE__);
        }
        else if(len_remain == 0)         //目的计算机端的socket关闭
        {
            return 0;
        }
        pread = recv_buf;          //重新初始化pread指针
    }

    //从自定义缓冲区读取一次数据
    for(i = 0; *pread != '\n'; i++)
    {
        if(i > len)         //防止指针越界
        {
            return -1;
        }
        date_buf[i] = *pread++;
        len_remain--;
    }
    //去除结束标志
    len_remain--;
    pread++;

    return i;       //读取成功
}