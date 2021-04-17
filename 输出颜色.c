#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
//#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

int main()
{
    while(1)
    {
        char name[100];
        char *arg;
        char *msg = (char *)malloc(sizeof(char)*100);
        char *colormsg = (char *)malloc(sizeof(char)*100);
        char *str = "ycl@ycl-PC:~",colorstr[20];
        getcwd(msg, 100);
        //sprintf(colormsg,"\033[%dm%s\033[0m",34,msg);
        //sprintf(colorstr,"\033[%dm%s\033[0m",31,str);
        sprintf(name,"\001\033[%dm%s\033[0m\002 \001\033[%dm%s\033[0m\002",31,str,34,msg);
        //printf("%s%s$ ",colorstr,colormsg);
        //printf("%s",name);
        if(arg)
        {
            free (arg);
            arg = (char *)NULL;
        }
        arg = readline(name);

        if(arg && *arg)
        {
            add_history(str);
        }
    }

    return 0;
}


