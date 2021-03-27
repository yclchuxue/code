#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_key_t key;

void * thread2(void *arg)
{
    long tsd = 5;
    printf("thread %d is running\n",pthread_self());
    pthread_setspecific(key, (void*)tsd);
    printf("thread %d returns %d\n",pthread_self(),pthread_getspecific(key));
}

void *threadl(void *arg)
{
    long tsd = 0;
    pthread_t thid2;

    printf("thread %d is running\n",pthread_self());
    pthread_setspecific(key, (void*)tsd);
    pthread_create(&thid2, NULL, thread2,NULL);
    sleep(5);
    printf("thread %d returns %d\n",pthread_self(),pthread_getspecific(key));
}

int main(void)
{
    int status;
    pthread_t thid1;
    printf("main thread begins runing\n");
    pthread_key_create(&key, NULL);
    pthread_create(&thid1, NULL,threadl,NULL);
    sleep(3);
    pthread_join(thid1,(void*)&status);                       //等待线程thid1结束
    pthread_key_delete(key);
    printf("main thread exit\n");

    return 0;
}
