/***************************************
 * 哲学家吃饭
 * 解法2：奇数先拿左边，偶数先那右边
 * ************************************/
#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h>
#include <semaphore.h> 

sem_t sem[6];

void thread_1(void *arg)
{
    int sum = 0;
    sem_wait(&sem[1]);
    printf("111拿起1筷子\n");
    sem_wait(&sem[2]);
    printf("111拿起2筷子\n");
    printf("\n哲学家1吃完！\n");
    sem_post(&sem[1]);
    printf("111释放1筷子\n");
    sem_post(&sem[2]);
    printf("111释放2筷子\n");
}

void thread_2(void *arg)
{
    sem_wait(&sem[3]);
    printf("222拿起3筷子\n");
    sem_wait(&sem[2]);
    printf("222拿起2筷子\n");
    printf("\n哲学家2吃完!\n");
    sem_post(&sem[3]);
    printf("222释放3筷子\n");
    sem_post(&sem[2]);
    printf("222释放2筷子\n");
}

void thread_3(void *arg)
{
    sem_wait(&sem[3]);
    printf("333拿起3筷子\n");
    sem_wait(&sem[4]);
    printf("333拿起4筷子\n");
    printf("\n哲学家3吃完!\n");
    sem_post(&sem[3]);
    printf("333释放3筷子\n");
    sem_post(&sem[4]);
    printf("333释放4筷子\n");
}

void thread_4(void *arg)
{
    //printf("*");
    sem_wait(&sem[5]);
    printf("444拿起5筷子\n");
    sem_wait(&sem[4]);
    printf("444拿起4筷子\n");
    //printf("A");
    printf("\n哲学家4吃完!\n");
    sem_post(&sem[5]);
    printf("444释放5筷子\n");
    sem_post(&sem[4]);
    printf("444释放4筷子\n");
}

void thread_5(void *arg)
{
    //printf("**");
    sem_wait(&sem[5]);
    printf("555拿起5筷子\n");
    //printf("AA");
    sem_wait(&sem[1]);
    printf("555拿起1筷子\n");
    printf("\n哲学家5吃完!\n");
    sem_post(&sem[5]);
    printf("555释放5筷子\n");
    sem_post(&sem[1]);
    printf("555释放1筷子\n");
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