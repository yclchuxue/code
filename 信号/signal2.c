/*****************************************************
 * 用signal函数接收指定信号，并设置函数响应
 * ***************************************************/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void hand(int num)
{
    sleep(3);
    printf("\n<ctrl+ C>\n");
}

int main()
{
    signal(SIGINT, hand);           //防止ctrl + C杀死程序

    while(1)                           //制造死循环
    {
        ;
    }

    return 0;
}