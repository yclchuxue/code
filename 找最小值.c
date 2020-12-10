#include <stdio.h>
int A[1000];
int main()
{
    int n;
    scanf("%d",&n);
    for(int i = 0;i<n;i++)
    {
        scanf("%d",&A[i]);
    }
    int min = A[0];
    for(int j = 1;j<n;j++)
    {
        if(A[j]<min)
        {
            min = A[j];
        }
    }
    printf("%d",min);

    return 0;
}