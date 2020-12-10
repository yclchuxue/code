#include <stdio.h>
int A[1000];

void maopao(int A[], int n)
{
    for(int i = 0;i<n-1;i++)
    {
        for(int j = 0;j<n-i-1;j++)
        {
            if(A[j]>A[j+1])
            {
                int t = A[j];
                A[j] = A[j+1];
                A[j+1] = t;
            }
        }
    }
}

int main()
{
    printf("请输入你要排序的数以-1结束：");
    int n, i = 0;
    do
    {
        scanf("%d",&n);
        if(n != -1)
        {
            A[i] = n;
            i++;
        }
    }while(n != -1);
    maopao(A,i);
    for(int j = 0;j<i;j++)
    {
        printf("%d\t",A[j]);
    }

    return 0;
}