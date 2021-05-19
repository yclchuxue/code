/**********************************
 * 一个生产者和一个消费者
 * *******************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct Listnode{
    int value;
    struct Listnode *next;
};

struct Listnode *head = NULL;

pthread_mutex_t lock1;// = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1; // = PTHREAD_COND_INITIALIZER;

void producter(void *arg)
{
    while(1)
    {   
        struct Listnode *p = malloc(sizeof(struct Listnode));
        p->value = rand()%1000 + 1;
        printf("producter——————————————————>%d\n", p->value);              
        pthread_mutex_lock(&lock1);
        p->next = head;
        head = p;
        
        pthread_mutex_unlock(&lock1);
        pthread_cond_signal(&cond1);
        sleep(rand() % 5);
    }
}

void consumer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock1);
        struct Listnode *q;
        while(head == NULL)
        {
            pthread_cond_wait(&cond1, &lock1);            //阻塞消费者线程，等待生产者生产产品
        }
        q = head;
        printf("consumer--->%d\n",q->value);
        head = q->next;
        free(q);

        pthread_mutex_unlock(&lock1);
        sleep(rand() % 5);
    }
}

int main()
{
    srand(time(NULL));  
    pthread_mutex_init(&lock1, NULL);    //初始化互斥锁
    pthread_cond_init(&cond1, NULL);     //初始化条件变量

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