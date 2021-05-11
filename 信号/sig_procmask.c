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

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)   //将newmask加入信号屏蔽集
    //if(sigprocmask(SIG_UNBLOCK,&newmask,&oldmask) < 0)   //将删除信号品比集中newmask中的内容
    {
        my_err("sigprocmask",__LINE__);
    }
    else
    {
        printf("SIGINT blocked\n");
    }

    sleep(10);

    sigpending(&zeromask);             //将为决信号放如zeromask
    if(sigismember(&zeromask, SIGINT) == 1)
    {
        printf("SIGINT is in pending queue\n");
    }
    else
    {
        printf("SIGINT is not in pending queue\n");
    }
    
    if(sigprocmask(SIG_UNBLOCK,&newmask,&oldmask) < 0)   //将删除信号品比集中newmask中的内容
    {
        my_err("sigprocmask",__LINE__);
    }
    else
    {
        printf("SIGINT unblocked\n");
    }

    while(1)
        ;

    return 0;
}