#include<stdio.h>

int main()
{
    int t,n;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        if(n%5==2||n%5==0)
        {
            printf("Cow\n");
        }
        else
        {
            printf("Pig\n");
        }
    }
    return 0;
}