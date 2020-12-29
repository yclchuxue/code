#include <stdio.h>

int searchInsert(int* nums, int numsSize, int target)
{
    if(nums[0]>=target)
    {
        return 0;
    }
    int i = 1, j = 1;
    for(;i<numsSize;i++)
    {
        if(nums[i] >= target)
        {
            return i;
        }
    }
    return numsSize;
}
