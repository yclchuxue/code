#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    printf("进程学习\n");
    pid = fork();
    switch(pid)
    {
        //getpid函数和getppid函数分别用于获得所在进程ID和所在进程父进程的ID
        case 0:
            printf("子进程正在运行,fork返回值为：%d, 父进程ID为 %d,子进程ID为 %d\n",pid,getppid(),getpid());
            break;
        case -1:
            perror("Peocess creation failed\n");
            break;
        default:
            printf("父进程正在运行，fork返回值为：%d,父进程ID为 %d\n",pid, getpid());
            break;
    }
    exit(0);
}