#include <stdio.h>
int A[1000];

void swap(int A[], int i, int j)
{
    int t = A[i];
    A[i] = A[j];
    A[j] = t;
}

void charu(int A[], int n)
{
    for(int i=1; i<n; i++)
    {
        int get = A[i];
        for(int j = i-1;j>=0;j--)
        {
            if(get<A[j])
            {
                swap(A,j+1,j);
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
    charu(A,i);
    for(int j = 0;j<i;j++)
    {
        printf("%d\t",A[j]);
    }

    return 0;
}