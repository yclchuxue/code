#include <stdio.h>
int main()
{
    int n;
    scanf("%d",&n);
    int a = 1;
    int b = 2;
    if(n == 1)return a;
    if(n == 2)return b;
    int c, i = 3;
    for(;i<=n;i++)
    {
        c = a+b;
        a = b;
        b = c;
    }
    printf("%d",c);

    return 0;
    
}