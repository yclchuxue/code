#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[],char **environ)
{
    int fd[2];
    pid_t pid;
    int stat_val;

    if(argc < 2)
    {
        printf("wrong parameters \n");
    }
    if(pipe(fd))                               //定义一个管道fd
    {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();                             //创建子进程
    switch(pid)
    {
        case -1:                              //创建失败
        {        
            perror("fork failed!\n");
            exit(1);
        }
        case 0:                               //子进程运行中
        {
            close(0);
            dup(fd[0]);
            execve("text",(void*)argv,environ);
            exit(0);
        }
        default:                              //父进程运行中
        {
            close(fd[0]);
            write(fd[1], argv[1],strlen(argv[1]));
            break;
        }
    }

    wait(&stat_val);
    exit(0);
}