/****************************************************************
 * 无管道符和重定向符的shell
 * *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>

void cvp(char **arg)
{
    int status;
    pid_t pid;
    if((arg[0] != NULL) && (strcmp(arg[0], "cd") == 0))
    {
        if(arg[1] == NULL)
        {
            return ;
        }
        if(strcmp(arg[1], "~") == 0)                         //cd ~回到用户目录
        {
            strcpy(arg[1], "/home/ycl/");
        }
        if(chdir(arg[1]) ==  -1)                             //chdir将当前工作目录该为参数工作目录
        {
            perror("cd");
        }
        return ;
    }
    pid = vfork();         //子进程先运行
    switch (pid)
    {
        case 0:
            //子进程运行中
            execvp(arg[0], arg);
            exit(0);
        case -1:
            //进程创建失败

        default:
            waitpid(pid, &status, 0);
            //父进程运行中
    }
}

void print()
{
    char *msg = (char *)malloc(sizeof(char)*100);
    getcwd(msg, 100);
    printf("%s:",msg);
    free(msg);
}

int main()
{
    while(1)
    {
        char *argv[5],B[5][10];
        char A[100],ch;
        int i = 0,j = 0,k = 0,num;
        print();
        ch = getchar();
        if(ch == '\n')
        {
            continue;
        }
        while(ch != '\n')
        {
            A[i] = ch;
            i++;
            ch = getchar();
        }
        A[i] = '\0';
        if(strcmp(A,"exit") == 0)
        {
            break;
        }
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
        cvp(argv);
    }
    return 0;
}