#include <stdio.h>
int main()
{
    int A[10] = {9,5,1,3,7,2,6,4,8,0};
    int B[10] = {0};
    for(int i = 0;i<10;i++)
    {
        B[A[i]]++;
    }
    for(int j = 0;j<10;j++)
    {
        while(B[j])
        {
            printf("%d\t",j);
            B[j]--;
        }
    }

    return 0;
}