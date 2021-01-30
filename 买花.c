#include <stdio.h>
#include <math.h>
int main()
{
    int n,A[100] = {0};
    scanf("%d",&n);
    for(int i = 0;i<n;i++)
    {
        int x;
        scanf("%d",&x);
        for(int j = 1;j<x/2;j++)
        {
            for(int h = 2;pow(2,h)<x/j+1;h++)
            {
                int y = pow(2,h);
                if(j * (y-1) == x)
                {
                    A[i] = 1;
                    break;
                }
            }
            if(A[i] == 1)
            {
                break;
            }
        }
    }
    for(int i = 0;i<n;i++)
    {
        if(A[i] == 1)
        {
            printf("YE5\n");
        }
        else
        {
            printf("NO\n");
        }
    }
    
    return 0;
}