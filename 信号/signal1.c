/*****************************************************
 * 用signal函数忽略<ctrl + C>产生的SIGINT函数，来防止被杀死
 * ***************************************************/
#include <stdio.h>
#include <signal.h>

int main()
{
    signal(SIGINT, SIG_IGN);           //防止ctrl + C杀死程序

    while(1)                           //制造死循环
    {
        printf("A\n");
    }

    return 0;
}