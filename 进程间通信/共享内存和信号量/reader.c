/*************************************************
 * 先运行writer.c文件，在writer：后输入内容到共享内存区
 * 再运行reader.c文件，读出共享内存区数据
 * **********************************************/
#include "sharemem.h"

int main()
{
    int       semid,shmid;
    char      *shmaddr;

    if((shmid = createshm(".",'m',SHM_SIZE)) == -1)      //打开共享内存区
    {
        exit(1);
    }

    if((shmaddr = shmat(shmid,(char *)0,0)) == (char *)-1)   //返回指向共享内存区的指针
    {
        perror("attach shared memory error!\n");
        exit(1);
    }

    if((semid = opensem(".",'s')) == -1)    //打开信号集
    {
        exit(1);
    }

    while(1)
    {
        printf("reader: ");
        wait_sem(semid, 0);         //等待信号为1
        sem_P(semid, 0);            //信号-1

        printf("%s\n",shmaddr);
        sleep(10);

        sem_V(semid, 0);            //信号+1
        sleep(10);
    }
}