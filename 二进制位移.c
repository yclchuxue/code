#include <stdio.h>
int main()
{
    unsigned int a = 14;//无符号型
    int b = a<<1;//左移n位相当与乘2的n次方
    printf("%d\n",b);
    int c = a>>1;//右移n位相当与除以2的n次方
    printf("%d\n",c);

    return 0;
}