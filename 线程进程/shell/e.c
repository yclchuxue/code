#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>

int main()
{
    char *A[5];
	char *arg[3];
	arg[0] = A[0] = "ls";
    arg[1] = "-l";
	arg[2] = NULL;
	int fp = open("text.txt",O_RDWR|O_CREAT|O_APPEND);     //打开文件
	dup2(fp,1);                                             //文件描述符改为1（输出）
	execvp(arg[0], arg);                                    //注意在shell中必须在子进程中执行
	return 0;
}