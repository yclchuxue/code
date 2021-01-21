#include <stdio.h>

int add(int a, int b){
    while(b)
    {           //特别注意，由于题目中提到了可能为负数，int型的第32位为符号位，如果负数与操作后左移，会导致溢出，所以要将与运算结果先转换为无符号，再进行左移操作
        int diff = (unsigned int)(a & b) << 1;
        a = a ^ b;
        b = diff;
    }

    return a;
}

int main()
{
    int m,n;
    printf("请输入两个数：\n");
    scanf("%d%d",&m,&n);
    int sum = add(m,n);
    printf("两数之和为：%d",sum);

    return 0;
}