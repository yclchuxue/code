#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
    while(1)
    {
        char *str = "myshell:$";
        str = readline(str);
        printf("%s\n", str);
        if(strcmp(str, "exit") == 0)
        {
            //输入exit退出程序
            break;
        }
        if(str && *str)
        {
            add_history(str);
        }
        free(str);
    }
}