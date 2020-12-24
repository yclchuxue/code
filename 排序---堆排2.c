#include <stdio.h>

void swap(int A[], int i, int j)
{
    int t = A[i];
    A[i] = A[j];
    A[j] = t;
}

void duipai(int A[],int n)
{
    int k = 0,h = n;
    while(k<h)
    {
        for(int i = n-1;i>=0;i--)
        {
            int s = (i-1)/2;
            if(A[s] < A[i])
            {
                swap(A, i, s);
            }
        }
        swap(A, 0, n-1);
        n = n - 1;
        k++;
    }
}

int main()
{
    int A[100] = {0};
    int n;
    scanf("%d",&n);
    for(int i = 0;i<n;i++)
    {
        scanf("%d",&A[i]);
    }
    duipai(A,n);
    for(int j = 0;j<n;j++)
    {
        printf("%d\t",A[j]);
    }

    return 0;
}