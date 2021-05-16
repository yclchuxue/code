/*************************************************
 * 用管道创建单向传递功能
 * **********************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void read_from_pipe(int fd)       //读管道
{
    char message[100];
    read(fd, message, 100);
    printf("read from pipe:%s",message);
}

void write_to_pipe(int fd)       //写管道
{
    char *message = "Hello, pipe!\n";
    write(fd, message,strlen(message)+1);
}

int main()
{
    int fd[2];
    pid_t pid;
    int stat_val;

    if(pipe(fd))
    {
        printf("create pipe failed!\n");
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
            close(fd[1]);      //子进程关闭fd1
            read_from_pipe(fd[0]);
            exit(0);
        }
        default:
        {
            close(fd[0]);             //父进程关闭fd2
            write_to_pipe(fd[1]); 
            wait(&stat_val);          //等待子进程
            exit(0);
        }
    }

    return 0;
}