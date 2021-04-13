#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

int cont[3];

void tj(char **arg)
{
    char *file;
    memset(cont, 0, sizeof(int)*4);
    int i = 0,sum,j = 0,k = 1,fd,sun = 0;
    int *status = NULL,*status2 = NULL,fd2,pid2;
    pid_t pid;
    while(arg[j] != NULL)
    {
        i = 0;
        while(arg[j][i] != '\0')
        {
            if(arg[j][i] == '<')
            {
                cont[0]++;
            }
            if(arg[j][i] == '>')
            {
                cont[1]++;
            }
            if(arg[j][i] == '|')
            {
                cont[2]++;
            }
            if(arg[j][i] == '&')
            {
                arg[j][i] = '\0';
                cont[3]++;
            }
            i++;
        }
        if(cont[0] + cont[1] + cont[2] + cont[3] > 0 && sun == 0)   //找到第一次出现重定向符和管道符位置
        {
            sun = cont[0] + cont[1] + cont[2] + cont[3];
            k = j;
        }
        j++;
    }
    sum = cont[0] + cont[1] + cont[2] + cont[3];
    if(sum != sun)
    {
        printf("wrong多\n");
    }
    if(sum == cont[3])
    {
        sum = 0;
    }

    pid = fork();         //子进程先运行
    if(sum == 0)
    {
        if(pid == 0)
        {
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
            execvp(arg[0], arg);
        }
    }
    if(sum == cont[0] && cont[0] == 1)          //只有一个输入重定向符 <
    {
        if(pid == 0)
        {
            file = arg[k+1];
            arg[k] = NULL;
            fd = open(file, O_RDONLY);
            dup2(fd,0);
            execvp(arg[0], arg);
        }
    }
    if(sum == cont[1] && cont[1] == 1)           //只有一个输出重定向符 >
    {
        if(pid == 0)
        {
            file = arg[k+1];
            arg[k] = NULL;
            fd = open(file,O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fd, 1);
            execvp(arg[0], arg);
        }
    } 
    if(sum == cont[1] && cont[1] == 2)           //有两个输出重定向符 >>
    {
        if(pid == 0)
        {
            file = arg[k+1];
            arg[k] = NULL;
            fd = open(file,O_RDWR | O_CREAT | O_APPEND);
            dup2(fd,1);
            execvp(arg[0], arg);
        }
    }
    if(sum == cont[2] && cont[2] == 1)           //只有一个管道符 |
    {
        if (pid == 0)
        {
            if( (pid2 = fork()) < 0 )
            {
                printf("创建进程2失败\n");
                return ;
            }
            else if(pid2 == 0)
            {
                arg[k] = NULL;
                fd2 = open("text", O_WRONLY|O_CREAT|O_TRUNC, 0644);
                dup2(fd2, 1);
                execvp(arg[0], arg);
                exit(0);
            }
            if(waitpid(pid2, status2, 0) == -1)
            {
                printf("wait for child process error\n");
            }
            fd2 = open("text", O_RDONLY);
            dup2(fd2, 0);
            execvp (arg[k+1], arg);
            if( remove("text"))
            {
                printf("remove error\n");
            }
            exit(0);
        }
        
    }
    if(sum = cont[3] && cont[3] == 1)    //后台运行符 & ，支持如： ls&  和 ls &   和   ls  &
    {

        printf("process id = %d\n", pid);
        return;
    }

    if(waitpid(pid, status, 0) == -1)             //等待子进程结束
    {
        printf("wrong等\n");
    }
}

void print()
{
    char *msg = (char *)malloc(sizeof(char)*100);
    char *colormsg = (char *)malloc(sizeof(char)*100);
    char *str = "ycl@ycl-PC:~",colorstr[20];
    getcwd(msg, 100);
    sprintf(colormsg,"\033[%dm%s\033[0m",34,msg);
    sprintf(colorstr,"\033[%dm%s\033[0m",31,str);
    printf("%s%s$ ",colorstr,colormsg);
    free(msg);
    free(colormsg);
}

int main()
{
    signal(SIGINT, SIG_IGN);
    while(1)
    {
        char *argv[5],B[5][10];
        char A[100],ch;
        int i = 0,j = 0,k = 0,num;
        print();                        //打印当前工作位置
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
        /*
        if(strcmp(A,"exit") == 0)
        {
            return 0;
        }
        */
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
        if(strcmp(argv[0], "exit") == 0)
        {
            break;
        }
        tj(argv);
    }
    return 0;
}