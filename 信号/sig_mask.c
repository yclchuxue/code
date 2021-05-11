#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void my_err(const char *err_string, int line)
{
    fprintf(stderr, "line:%d ",line);
    perror(err_string);
    exit(1);
}

void hander_sigint(int signo)
{
    printf("\nrecv SIGINT\n");
}

int main()
{
    sigset_t newmask, oldmask, pendmask;    //定义信号集

    if(signal(SIGINT,hander_sigint) ==  SIG_ERR)
    {
        my_err("signal",__LINE__);
    }

    sleep(10);                                       //留出按<ctrl + C>的时间

    sigemptyset(&newmask);                           //初始化一个信号集，不包括任何信号
    sigaddset(&oldmask,SIGINT);                      //向oldmask信号集中添加SIGINT信号

    if(sigprocmask(SIG_UNBLOCK, &newmask,&oldmask) < 0)
    {
        my_err("sigprocmask",__LINE__);
    }
    else
    {
        printf("SIGINT blocked\n");                //SIGINT阻塞
    }
    
    sleep(10);                                     //留出按<ctrl + C>的时间

   if(sigpending(&pendmask) < 0)                   //将未决信号集返回给pendmask信号集
    {
        my_err("sigpending",__LINE__);
    }

    switch(sigismember(&pendmask, SIGINT))         //检测信号是否在pendmask信号集中
    {
        case 0:
        {
            printf("SIGINT is not in pending queue\n");
            break;
        }
        case 1:
        {
            printf("SIGINT is in pending queue\n");
            break;
        }
        case -1:
        {
            my_err("sigismember",__LINE__);
            break;
        }
        default:
            break;
    }

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)                  //将信号屏蔽码设置为oldmask信号集
    {
        my_err("sigprocmask",__LINE__);
    }
    else
    {
        printf("SIGINT unblocked\n");                  //SIGINT没有阻塞
    }

    while(1)
        ;
    
    return 0;
}