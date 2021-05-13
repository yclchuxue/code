#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int globVar = 5;

int ma()
{
    pid_t pid;
    int var = 1;

    printf("fork与vfork \n");

    //pid = fork();
    pid = vfork();
    switch(pid)
    {
        case 0:
            printf("子进程运行中\n");
            globVar = globVar +5;
            var = var + 5;
            printf("Child's globVar = %d,var = %d\n",globVar,var);
            exit(0);
        case -1:
            perror("创建失败\n");
            exit(0);
        default :
            printf("父进程运行中\n");
            globVar = globVar + 5;
            var = var + 5;
            printf("Parent's globVar = %d ,var = %d\n",globVar ,var);
            exit(0);
    }
}

int main()
{
    ma();

    return 0;
}