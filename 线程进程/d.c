#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int *get_thid(void);

int main(int argc,char **argv)
{
    pthread_t thid;                     //声明进程ID变量

    printf("parent pthread is me,my thid is %lu\n",pthread_self( ));
    if(pthread_create(&thid,NULL,(void *)get_thid,NULL) != 0)
    {
        printf("Error!\n");                 //调用函数进行进程的创建
        return 0;
    }
    sleep(1);
    return 0;
}
int *get_thid(void)                      //创建进程时,被调用的函数
{
    pthread_t thid;

    thid = pthread_self( );
    if(thid < 0)
    {
        printf("Error!\n");
        exit(0);
    }
    printf("I'm child pthread,my thid is %lu\n",thid);
    return NULL;
}
