/*************************************
 * 先运行procwrite.c文件，写入有名管道
 * 后运行na_pipe.c文件，读出有名管道中的内容
 * 如果已经有了有名管道myfifo则先删除它
 * **********************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME       "myfifo"
#define BUF_SIZE        1024

int main()
{
    int fd;
    char buf[BUF_SIZE];

    umask(0);                           //给予最大权限
    fd = open(FIFO_NAME, O_RDONLY);      //以只读的方式打开文件myfifo
    read(fd, buf, BUF_SIZE);             //读取文件myfifo
    printf("Read content: %s\n", buf);
    close(fd);                           //关闭文件

    return 0;
}