#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int a = 0;

void *thread(void *arg)
{
    a = 10;
    printf("%d    %d\n",a,arg);
}

int main()
{
    long b = 3;
    pthread_t thid1;
    printf("%d    %d\n",a,b);
    if(pthread_create(&thid1, NULL,thread,(void*)b) != 0)       //只能传入一个局部变量
    {
        printf("创建失败\n");
        return 0;
    }
    pthread_join(thid1, NULL);
    a = 5;
    printf("%d\n",a);
    return 0;
}