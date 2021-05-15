#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int *thread(void*arg)
{
    pthread_t newthid;

    newthid = pthread_self();
    printf("新建线程ID：%u\n", newthid);
    return NULL;
}

int main()
{
    pthread_t thid;

    printf("主线程ID：%u\n",pthread_self());
    int per = pthread_create(&thid,NULL,(void *)thread,NULL);
    printf("%d\n",per);
    if(per != 0)
    {
        printf("创建失败\n");
        exit(1);
    }

    sleep(1);

    exit(0);
}