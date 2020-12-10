#include <stdio.h>
int main()
{
    int A[10] = { 5,4,6,7,8,9,2,1,3,0 };
    for(int i = 0;i<10 - 1;i++)
    {
        for(int j = 0;j<10 - i - 1;j++)
        {
            if(A[j] > A[j + 1])
            {
                int t = A[j];
                A[j] = A[j + 1];
                A[j + 1] = t;
            }
        }
    }
    for(int i = 0;i<10;i++)
    {
        printf("%d\t",A[i]);
    }
    return 0;
}