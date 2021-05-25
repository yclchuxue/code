/***************************************
 * 线程几乎在同一时间运行
 * ************************************/
#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h>
#include <semaphore.h> 

sem_t sem[6];

void thread_1(void *arg)
{
    time_t now;
    struct tm *ptm;
    //time() returns the time as the number of seconds since the Epoch,
    //1970-01-01 00:00:00 +0000 (UTC)
    //这里只能得到当前距离某个时间的总秒数，需要进一步转换
    time (&now);
    //获取当地日期和时间
    ptm = localtime (&now);
    //将转换后的时间以字符串形式显示
    printf ("now: %s", asctime(ptm));
}

void thread_2(void *arg)
{
    time_t now;
    struct tm *ptm;
    //time() returns the time as the number of seconds since the Epoch,
    //1970-01-01 00:00:00 +0000 (UTC)
    //这里只能得到当前距离某个时间的总秒数，需要进一步转换
    time (&now);
    //获取当地日期和时间
    ptm = localtime (&now);
    //将转换后的时间以字符串形式显示
    printf ("now: %s", asctime(ptm));
}

void thread_3(void *arg)
{
    time_t now;
    struct tm *ptm;
    //time() returns the time as the number of seconds since the Epoch,
    //1970-01-01 00:00:00 +0000 (UTC)
    //这里只能得到当前距离某个时间的总秒数，需要进一步转换
    time (&now);
    //获取当地日期和时间
    ptm = localtime (&now);
    //将转换后的时间以字符串形式显示
    printf ("now: %s", asctime(ptm));
}

void thread_4(void *arg)
{
    time_t now;
    struct tm *ptm;
    //time() returns the time as the number of seconds since the Epoch,
    //1970-01-01 00:00:00 +0000 (UTC)
    //这里只能得到当前距离某个时间的总秒数，需要进一步转换
    time (&now);
    //获取当地日期和时间
    ptm = localtime (&now);
    //将转换后的时间以字符串形式显示
    printf ("now: %s", asctime(ptm));
}

void thread_5(void *arg)
{
    time_t now;
    struct tm *ptm;
    //time() returns the time as the number of seconds since the Epoch,
    //1970-01-01 00:00:00 +0000 (UTC)
    //这里只能得到当前距离某个时间的总秒数，需要进一步转换
    time (&now);
    //获取当地日期和时间
    ptm = localtime (&now);
    //将转换后的时间以字符串形式显示
    printf ("now: %s", asctime(ptm));
}

int main()
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    pthread_t thread5;

    sem_init(&sem[1], 0, 1);
    sem_init(&sem[2], 0, 1);
    sem_init(&sem[3], 0, 1);
    sem_init(&sem[4], 0, 1);
    sem_init(&sem[5], 0, 1);

    int ret;

    //printf("%d\n%d\n%d\n%d\n%d\n",sem[1],sem[2],sem[3],sem[4],sem[5]);

    ret = pthread_create(&thread1, NULL, (void *)thread_1, NULL);
    if(ret != 0)
    {
        printf("Thread_1 wrong!\n");
    }

    ret = pthread_create(&thread2, NULL, (void *)thread_2, NULL);
    if(ret != 0)
    {
        printf("Thread_2 wrong!\n");
    }

    ret = pthread_create(&thread3, NULL, (void *)thread_3, NULL);
    if(ret != 0)
    {
        printf("Thread_3 wrong!\n");
    }

    ret = pthread_create(&thread4, NULL, (void *)thread_4, NULL);
    if(ret != 0)
    {
        printf("Thread_4 wrong!\n");
    }

    ret = pthread_create(&thread5, NULL, (void *)thread_5, NULL);
    if(ret != 0)
    {
        printf("Thread_5 wrong!\n");
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);

    sem_destroy(&sem[1]);
    sem_destroy(&sem[2]);
    sem_destroy(&sem[3]);
    sem_destroy(&sem[4]);
    sem_destroy(&sem[5]);


    return 0;
}