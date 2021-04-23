#include <stdio.h>

int main()
{
    int m,i,j;
    int A[100],B[100];
    scanf("%d",&m);
    for(i = 0; i < m; i++)
    {
        scanf("%d/%d",&A[i],&B[i]);
    }
    for(i = 0; i < m; i++)
    {
        printf("%d/%d ",&A[i],&B[i]);
    }
    return 0;
}