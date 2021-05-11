#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int i, j;
    int signum = SIGTERM; 
    pid_t pid;
    printf("%d\n",argc);

    //检查参数个数
    if(argc != 2 && argc != 4)
    {
        printf("Usage: ./my_kill <-s signum> {PID}\n");
        exit(0);
    }

    //从命令行参数解析出信号编码
    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            signum = atoi(argv[i+1]);
            break;
        }
    }

    //解析出进程号
    if(argc == 2)
    {
        pid = atoi(argv[1]);
    }
    else
    {
        for(j = 1; j<argc;j++)
        {
            if(j != i && j != i+1)
            {
                pid = atoi(argv[j]);
                break;
            }
        }
    }

    if(kill(pid, signum) < 0)
    {
        perror("kill");
        exit(1);
    }
    
    return 0;
}