#include<stdio.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>

int main() 
{
    char* argv[] = {"ls", "-l", NULL}; // 构造 vector，注意argv[0] 是占用参数
    if (execvp(argv[0], argv) == -1) 
    {                                      // 替换代码段和数据段，并重新从 ls 入口点执行
        perror("exec");
        return 1;
    }

    return 0;  
}