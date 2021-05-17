#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

#define SHM_SIZE 1024

union semun{
    int               val;
    struct semid_ds   *buf;
    unsigned short    *array;
};

//创建信号量函数
int createsem(const char *pathname, int proj_id, int members, int init_val)
{
    key_t       msgkey;            //键值
    int         index,sid;
    union semun semopts;

    if((msgkey = ftok(pathname,proj_id)) == -1)     //获取键值
    {
        perror("ftok error!\n");
        return -1;
    }

    if((sid = semget(msgkey, members, IPC_CREAT|0666)) == -1)    //创建或打开信号集
    {
        perror("semget call failed!\n");
        return -1;
    }

    semopts.val = init_val;
    for(index = 0; index < members; index++)
    {
        semctl(sid, index, SETVAL, semopts);
    }
    return (sid);
}

//P操作函数   信号-1
int sem_P(int semid, int index)             //index = 0
{
    struct sembuf buf = {0, -1, IPC_CREAT};  //信号在信号集中的索引->0   
                                             //操作类型->-1   信号加上-1即-1
                                             //操作标志->IPC_CREAT

    if(index < 0)
    {
        perror("index of array cannot equals a minus value!");
        return -1;
    }

    buf.sem_num = index;                    //信号在信号集中的索引->0
    if(semop(semid , &buf, 1) == -1)        //信号-1
    {
        perror("a wrong operation to semaphore occurred!");
        return -1;
    }
    return 0;
}

//V操作函数   信号+1
int sem_V(int semid, int index)             //index = 0
{
    struct sembuf buf = {0, +1, IPC_CREAT};  //信号在信号集中的索引->0   
                                             //操作类型->+1   信号加上+1即+1
                                             //操作标志->IPC_CREAT

    if(index < 0)                            
    {
        perror("index of array cannot equals a minus value!\n");
        return -1;
    }

    buf.sem_num = index;                   //信号在信号集中的索引->0
    if(semop(semid,&buf, 1) == -1)         //信号+1
    {
        perror("a wrong operation to semaphore occurred!");
        return -1;
    }

    return 0;
}

//等待信号为1
int wait_sem(int semid, int index)            //index = 0
{
    while(semctl(semid, index, GETVAL,0) == 0)
    {
        sleep(1);
    } 

    return 1;
}

//创建共享内存函数
int createshm(char *pathname, int proj_id, size_t size)
{
    key_t       msgkey;            //键值
    int         sid;

    if((msgkey = ftok(pathname, proj_id)) == -1)    //获取键值
    {
        perror("ftok error!\n");
        return -1;
    }

    if((sid = shmget(msgkey, size, IPC_CREAT|0666)) == -1)   //创建或访问共享内存区
    {
        perror("shmget call failed!\n");
        return -1;
    }

    return sid;
}

//删除信号集函数
int sem_deleta(int semid)
{
    return (semctl(semid,0,IPC_RMID));
}

//打开信号量函数
int opensem(const char *pathname, int proj_id)
{
    key_t       msgkey;
    int         sid;

    if((msgkey = ftok(pathname, proj_id)) == -1)      //获取键值
    {
        perror("ftok error!\n");
        return -1;
    }

    if((sid = semget(msgkey,0, IPC_CREAT|0666)) == -1)  //创建或打开信号集
    {
        perror("semget call failed!\n");
        return -1;
    }

    return (sid);
}
