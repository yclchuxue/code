#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
    int i;
    for(i = 1; i <= 5; i++)
    {
        printf("key[%d] = %ul\n", i, ftok(".",i));
        //ftok函数根据键值生成唯一的键值
    }

    return 0;
}