/*********************
 * 练习
 * ******************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Listnode{
    int value;
    struct Listnode *next;
}ListNode;

ListNode *head = NULL;

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

void producter(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock1);                       
        pthread_cond_wait(&cond1, &lock1);
        sleep(1);
        printf("A");
        sleep(1);
        printf("p");
        pthread_mutex_unlock(&lock1);
        return;
    }

}

void consumer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock1);
        sleep(1);
        printf("B");
        sleep(1);
        printf("c");
        pthread_mutex_unlock(&lock1);
        pthread_cond_signal(&cond1);
        return;
    }
}

int main()
{
    printf("*");
    //pthread_mutex_init(&lock, NULL);    //初始化互斥锁
    //pthread_cond_init(&cond, NULL);     //初始化条件变量

    pthread_t pid, cid;
    int ret;
    ret = pthread_create(&pid, NULL, (void *)producter, NULL);
    if(ret != 0)
    {
        printf("Error: pthread_create producter!\n");
    }
    ret = pthread_create(&cid, NULL, (void *)consumer, NULL);
    if(ret != 0)
    {
        printf("Error: pthread_create consumer!\n");
    }

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_cond_destroy(&cond1);

    return 0;
}