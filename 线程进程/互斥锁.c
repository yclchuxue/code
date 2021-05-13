#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;

void *thread_1(void *data)
{
    pthread_mutex_lock(&lock);              //加锁
    pthread_cond_wait(&cond, &lock);        
    printf("A%s\n", __func__);
    pthread_mutex_unlock(&lock);
}

void *thread_2(void *data)
{
    pthread_mutex_lock(&lock);
    printf("B%s\n", __func__);
    pthread_mutex_unlock(&lock);

    pthread_cond_signal(&cond);
}

int main(int argc, char const *argv[])
{
    pthread_t pid[2];
    pthread_mutex_init(&lock, NULL);                   //初始化锁
    pthread_cond_init(&cond, NULL);                    //初始化条件变量

    pthread_create(&pid[0], NULL, thread_1, NULL);     //创建线程1
    pthread_create(&pid[1], NULL, thread_2, NULL);     //创建线程2

    pthread_join(pid[0], NULL);                        //等待线程结束
    pthread_join(pid[1], NULL);                        //等待线程结束

    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
