/***********************************
 * 创建一个消息队列，并写入消息
 * 编译运行该文件
 * 在shell下输入ipcs命令
 * 系统内部生成了一个消息队列Message Queues，其中含有一条消息
 * ********************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define BUF_SIZE             256
#define PROJ_ID              32
#define PATH_NAME            "."

int main()
{
    struct mymsgbuf{
        long msgtype;
        char ctrlstring[BUF_SIZE];
    }msgbuffer;
    int   qid;                          //消息队列标识符
    int   msglen;
    key_t msgkey;

    if((msgkey = ftok(PATH_NAME, PROJ_ID)) == -1)  //获取键值
    {
        perror("ftok error!\n");
        exit(1);
    }

    if((qid = msgget(msgkey, IPC_CREAT | 0660)) == -1)  //创建消息队列
    {
        perror("msgget error!\n");
        exit(1);
    }

    msgbuffer.msgtype = 3;
    strcpy(msgbuffer.ctrlstring, "Hello,message queue");
    msglen = sizeof(struct mymsgbuf) - 4;
    if(msgsnd(qid,&msgbuffer, msglen, 0) == -1)        //将msgbuffer结构中的信息写入消息中
    {
        perror("msgget error!\n");
        exit(1);
    }

    return 0;
}