#include <stdio.h>
#include <stdlib.h>

int cmp (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int digui(int *stones,int n)
{
    int i = n-1;
    if(stones[i] == stones[i-1])
    {
        stones[i] = 0;
        stones[i-1] = 0;
        n = n - 2;
    }
    else if(stones[i] > stones[i-1])
    {
        stones[i-1] = stones[i] - stones[i-1];
        stones[i] = 0;
        qsort(stones,n-1,sizeof(int),cmp);
        n = n - 1;
    }

    return n;
}

int lastStoneWeight(int* stones, int stonesSize)
{
    int n = stonesSize;
    qsort(stones,stonesSize,sizeof(int),cmp);
    while(n != 1 && n != 0)
    {
        n = digui(stones,n);
    }

    return stones[0];
}

int main()
{
    int n;
    scanf("%d",&n);
    int A[10];
    for(int i = 0;i < n;i++)
    {
        scanf("%d",&A[i]);
    }
    int ret = lastStoneWeight(A,n);

    printf("%d",ret);

    return 0;
}