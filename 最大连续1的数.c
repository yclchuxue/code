#include <stdio.h>

int findMaxConsecutiveOnes(int* nums, int numsSize) 
{
    int maxCount = 0, count = 0;
    for (int i = 0; i < numsSize; i++) 
    {
        if (nums[i] == 1) 
        {
            count++;
        } 
        else 
        {
            maxCount = fmax(maxCount, count);
            count = 0;
        }
    }
    maxCount = fmax(maxCount, count);
    return maxCount;
}


int main()
{
    int A[100] = {0};
    int n;
    scanf("%d", &n);
    for(int i = 0;i<n;i++)
    {
        scanf("%d", &A[i]);
    }
    int x = findMaxConsecutiveOnes(A,n);
    printf("%d", x);

    return 0;
}