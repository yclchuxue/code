/*********************
 * 生产者和消费者线程     copy
 * ******************/
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <errno.h>  
#include <pthread.h> 
 
void err_thread(int ret, char *str)  
{  
    if (ret != 0) 
    {  
        fprintf(stderr, "%s:%s\n", str, strerror(ret)); 
        pthread_exit(NULL);  
    }  
}  
 
 
/*链表作为公享数据,需被互斥量保护*/
struct msg {  
    struct msg *next;  
    int num;  
};  
 
/*头结点*/
struct msg *head;
 
/*静态初始化，条件变量和一个互斥锁*/
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
 
/*消费者*/
void *consumer(void *p)
{
 
    
    /*消费者循环消费, 创建和初始化已经在开始，静态定义*/
    while(1)
    {
        struct msg *mp;    
    printf("B");
         /*3. 上锁*/
        pthread_mutex_lock(&lock);
    printf("BB");
        /*头结点为空，没有结点解锁，解锁并阻塞等待*/
        /*4. 条件满足，处理数据，条件不满足阻塞数据*/
        while(head == NULL)
        {
    printf("C");
            /*解锁并阻塞等待*/
            pthread_cond_wait(&has_product, &lock);
        }
 
        /*5. 访问共享数据*/
        mp = head;        
        head = mp->next;                 //模拟消费掉一个产品 
 
        /*6. 解锁，释放条件变量，释放锁*/ 
        pthread_mutex_unlock(&lock);  
 
        printf("-Consume %lu---%d\n", pthread_self(), mp->num);  
        free(mp);  
        sleep(rand() % 5);   
    } 
 
    return NULL; 
}
 
/*生产者*/
void *producer(void *arg)
{
    /*定义结构体变量*/
    while(1)
    {
        struct msg *mp = malloc(sizeof(struct msg));       
        
         /*1.生产数据*/
        mp -> num = rand() %1000 + 1;         //产生一个随机数
        printf("-Produce %d\n", mp->num);
        /*头插法实现链表插入：生产的即为插入的结点*/
        
        /*2.加锁*/
        pthread_mutex_lock(&lock);
 
        /*3. 将数据发到公共区域，头插法加入链表*/
        mp->next = head;  
        head = mp;
 
        /*4. 解锁*/
        pthread_mutex_unlock(&lock);
 
        /*5. 通知唤醒阻塞在条件变量上的线程*/ 
        pthread_cond_signal(&has_product);
 
        sleep(rand() % 5);    //睡眠随机时间
    }
    return NULL;
}
 
int main(int argc, char *argv[])  
{  
    int ret;
    pthread_t pid, cid;  
    srand(time(NULL));  
  
    /*创建两个线程分别作为生产者和消费者*/
    ret = pthread_create(&pid, NULL, producer, NULL);     //创建生产者线程
	if (ret != 0)   
	    err_thread(ret, "pthread_create produser error");
 
    ret = pthread_create(&cid, NULL, consumer, NULL);     //创建消费者线程
    if(ret != 0)
	    err_thread(ret, "pthread_create produser error");
 
  
    /*释放两个线程*/
    pthread_join(pid, NULL);  
    pthread_join(cid, NULL);  
  
    return 0;  
}