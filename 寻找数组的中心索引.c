int pivotIndex(int* nums, int numsSize)
{
    if(numsSize == 0)
    {
        return -1;
    }
    int sum = 0;
    for(int i = 0; i < numsSize; i++)
    {
        sum = sum + nums[i];
    }
    int x = -1,sum1 = 0,sum2 = sum;
    for(int i = 0; i < numsSize; i++)
    {
        if(i>=1)
        {
            sum1 = sum1 + nums[i-1];
        }
        sum2 = sum2 - nums[i];
        if(sum1 == sum2)
        {
            x = i;
            break;
        }
    }

    return x;
}