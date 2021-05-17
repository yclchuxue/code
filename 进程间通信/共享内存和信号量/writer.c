/*************************************************
 * 先运行writer.c文件，在writer：后输入内容到共享内存区
 * 再运行reader.c文件，读出共享内存区数据
 * **********************************************/
#include "sharemem.h"

int main()
{
    int       semid,shmid;                      //semget和shmget的返回值
    char      *shmaddr;                         //shmat的返回值
    char      write_str[SHM_SIZE];

    if((shmid = createshm(".",'m', SHM_SIZE)) == -1)          //创建共享内存区
    {
        exit(1);
    }

    if((shmaddr = shmat(shmid, (char *)0, 0)) == (char *)-1)  //返回指向共享内存区的指针
    {
        perror("attach shared memory error!\n");
        exit(1);
    }

    if((semid = createsem(".",'s',1,1)) == -1)                //创建（打开）信号集
    {
        exit(1);
    }

    while(1)
    {
        wait_sem(semid, 0);         //等待信号为1
        sem_P(semid, 0);            //信号-1

        printf("writer: ");
        fgets(write_str,1024,stdin);        //输入内容
        int len = strlen(write_str)-1;
        write_str[len] = '\0';
        strcpy(shmaddr,write_str);          //写入共享内存区
        sleep(10);

        sem_V(semid, 0);                    //信号+1
        sleep(10);
    }
}
