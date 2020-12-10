#include <stdio.h>
int main()
{
    int k;
    scanf("%d",&k);
    int s = 1, sum = 0, i = 1;
    while(s<=k)
    {
        sum = sum + i*i;
        i++;
        s = s + i;
    }
    if(s+i>k)
    {
        sum = sum + (k-s+i)*i;
    }
    printf("%d",sum);

    return 0;
}