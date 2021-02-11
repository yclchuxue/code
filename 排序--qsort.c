#include <stdio.h>
#include <stdlib.h>

int cmp(int* a,int* b)
{
    return *a - *b;
}

int main()
{
    int A[10] = {0};
    for(int i = 0; i < 10; i++)
    {
        scanf("%d",&A[i]);
    }
    qsort(A,10,sizeof(int),cmp);
    for(int i = 0; i < 10; i++)
    {
        printf("%d\t",A[i]);
    }

    return 0;
}