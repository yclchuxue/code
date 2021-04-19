#include <stdio.h>
#include <string.h>

int removeElement(int* nums, int numsSize, int val)
{
    int i = 0;
    int j = 0;
    for(;j<numsSize;j++)
    {
        if(nums[j] != val)
        {
            nums[i] = nums[j];
            i++;
        }
    }

    return i;
}

int main()
{
    int nums[4] = {3,2,2,3};
    int numsSize = 4;
    numsSize =  removeElement(nums,numsSize,3);
    for(int i = 0; i < numsSize;i++)
    {
        printf("%d\t",nums[i]);
    }

    return 0;
}