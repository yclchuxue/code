#include <stdio.h>

int removeDuplicates1(int* nums, int numsSize)
{
    int m,n;
    int i;
    for(i = 0; i < numsSize;i++)
    {
        n = 0;
        if(numsSize <= 1)
        {
            return numsSize;
        }
        if(nums[i] != nums[i+1] && i+1 < numsSize)
        {
            n = 1;
        }
        for(int j = i+1; j < numsSize;j++)
        {
            if(nums[i] != nums[j] && i + 1 != j)
            {
                m = i;
                n = 1;
                for(int k = j;k< numsSize;k++)
                {
                    nums[m+1] = nums[k];
                    m++;
                }
                numsSize = numsSize-j+i+1;
                break;
            }
        }
        if(n == 0)
        {
            break;
        }
    }
    return i+1;
}

int removeDuplicates(int* nums, int numsSize)
{
    int i,j = 1;
    if(numsSize == 0) return numsSize;
    for(i = 1; i < numsSize; i++)
    {
        if(nums[i] == nums[i-1])
        {
            continue;
        }
        else
        {
            nums[j] = nums[i];
            j++;
        }
    }
    return j;
}

int main()
{
    int A[10] = {1,1,1,2,2,3,3,3,4,4};
    int n = removeDuplicates(A, 10);
    for(int i = 0; i < n; i++)
    {
        printf("%d\t", A[i]);
    }

    return 0;
}