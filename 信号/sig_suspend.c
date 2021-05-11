#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void my_err(const char * err_string, int line)
{
    fprintf(stderr, "line:%d ", line);
    perror(err_string);
    exit(1);
}

void handler_sigint(int signo)        //SIGINT信号处理函数
{
    printf("\nrecv SIGINT\n");
}

int main()
{
    sigset_t newmask, oldmask, zeromask;

    if(signal(SIGINT, handler_sigint) == SIG_ERR)
    {
        my_err("signal",__LINE__);
    }

    sigemptyset(&newmask);        //初始化一个空的信号集newmask
    sigemptyset(&zeromask);       //初始化一个空的信号集
    sigaddset(&newmask, SIGINT);  //向newmask信号集加入了SIGINT信号

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
    {
        my_err("sigprocmask",__LINE__);
    }
    else
    {
        printf("SIGINT blocked\n");
    }


    if(sigsuspend(&zeromask) != -1)           //取消所有的信号屏蔽，并等待信号
    {
        my_err("sigsuspend",__LINE__);
    }
    else
    {
        printf("recv a signo, return from sigsuspend\n");
    }

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
    {
        my_err("sigprocmask",__LINE__);
    }
    pause();

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
    {
        my_err("sigprocmask",__LINE__);
    }
    
    while(1)
        ;

    return 0;
}