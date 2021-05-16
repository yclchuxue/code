/*************************************************
 * 用两个管道创建双向传递功能
 * **********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void child_rw_pipe(int readfd, int writefd)
{
    char *message1 = "from child process!\n";
    write(writefd, message1,strlen(message1)+1);                    //写入管道

    char message2[100];
    read(readfd, message2,100);                                     //读管道
    printf("child process read from pipe:%s",message2);
}

void parent_rw_pipe(int readfd, int writefd)
{
    char *message1 = "from parent process!\n";
    write(writefd, message1,strlen(message1)+1);                    //写管道

    char message2[100];
    read(readfd, message2,100);
    printf("parent process read from pipe:%s",message2);            //读管道
}

int main()
{
    int pipe1[2],pipe2[2];
    pid_t pid;
    int stat_val;

    printf("realize full-duplex communication:\n\n");
    if(pipe(pipe1))                                                 //创建管道pipe1 父写子读
    {
        printf("pipe1 failed!\n");
        exit(1);
    }
    if(pipe(pipe2))                                                 //创建管道pipe2 父读子写
    {
        printf("pipe2 failed!\n");
        exit(1);
    }

    pid = fork();
    switch(pid)
    {
        case -1:
        {
            printf("fork error!\n");
            exit(1);
        }
        case 0:
        {
            //子进程
            close(pipe1[1]);                       //关闭写
            close(pipe2[0]);                       //关闭读
            child_rw_pipe(pipe1[0],pipe2[1]);
            exit(0);
        }
        default :
        {
            close(pipe1[0]);                       //关闭读
            close(pipe2[1]);                       //关闭写
            parent_rw_pipe(pipe2[0],pipe1[1]);
            wait(&stat_val);
            exit(0);
        }
    }
}