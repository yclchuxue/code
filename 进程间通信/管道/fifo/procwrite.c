/*************************************
 * 先运行procwrite.c文件，写入有名管道
 * 后运行na_pipe.c文件，读出有名管道中的内容
 * 如果已经有了有名管道myfifo则先删除它
 * **********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME       "myfifo"
#define BUF_SIZE        1024

int main()
{
    int fd;
    char buf[BUF_SIZE] = "有名管道";
    char A[100] = "ABC";

    umask(0);
    if(mkfifo(FIFO_NAME, S_IFIFO | 0666) == -1)          //创建一个有名管道
    {
        perror("mkfifo error!");
        exit(1);
    }
    if((fd = open(FIFO_NAME, O_WRONLY)) == -1)           //以写的方式打开一个有名管道
    {
        perror("open error!");
        exit(1);
    }
    write(fd, buf,strlen(buf)+1);                        //向管道内写入内容,并等待na_pipe进程读出myfifo中的内容
    write(fd,A,strlen(A)+1);                             //该内容没有被写入或者读出
    printf("na_pipe 读出完毕\n");
    
    close(fd);                                           //关闭
    
    return 0;
}