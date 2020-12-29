#include <stdio.h>
int main()
{
    int n, m = 1;
    scanf("%d",&n);
    for(int i = n;i>0;i--)
    {
        for(int j = 1;j<=i;j++)
        {

            if(m<10)
            {
                printf("%d",m);
            }
            else
            {
                printf("%d",m);
            }
            m++;
        }
        printf("\n");
    }
}