#include <stdio.h>
int A[1000];

void charu(int A[], int n)
{
    for(int i=1; i<n; i++)
    {
        int get = A[i];
        for(int j = i-1;j>=0;j--)
        {
            if(get<A[j])
            {
                A[j+1] = 
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