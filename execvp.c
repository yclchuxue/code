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


int main()
{
    char *argv[5],B[5][10];
    char A[100],ch;
    int i = 0,j = 0,k = 0,num;
    ch = getchar();
    while(ch != '\n')
    {
        A[i] = ch;
        i++;
        ch = getchar();
    }
    A[i] = '\0';
    num = i;
    for(i = 0; i < num ; i++)
    {
        j = 0;
        while(A[i] != ' ' && i < num)
        {
            B[k][j] = A[i];
            i++;
            j++;
        }
        B[k][j] = '\0';
        k++;
    }

    for(i = 0; i < k ; i++)
    {
        argv[i] = (char *)B[i];
    }
    argv[k] = NULL;

    execvp(argv[0], argv);

    return 0;
}