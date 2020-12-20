#include<stdio.h>

void swap(int A[][5],int k,int i,int j)
{
    int t = A[k][i];
    A[k][i] = A[k][j];
    A[k][j] = t;
}

void maopao(int A[][5],int n)
{
    for(int k = 0;k<5;k++)
    {
        for(int i = 0;i<n-1;i++)
        {
            for(int j = 0;j<n-i-1;j++)
            {
                if(A[k][j+1]<A[k][j])
                {
                    swap(A,k,j+1,j);
                }
            }
        }
    }
}

int main()
{
    int A[5][5];
    for(int i = 0;i<5;i++)
    {
        for(int j = 0;j<5;j++)
        {
            scanf("%d",&A[i][j]);
        }
    }
    maopao(A,5);
    for(int i = 0;i<5;i++)
    {
        for(int j = 0;j<5;j++)
        {
            printf("%d\t",A[i][j]);
        }
        printf("\n");
    }

    return 0;
}