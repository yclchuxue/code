double findMaxAverage(int* nums, int numsSize, int k) 
{
    int sum = 0;
    for (int i = 0; i < k; i++) 
    {
        sum += nums[i];
    }
    int maxSum = sum;
    for (int i = k; i < numsSize; i++) 
    {
        sum = sum - nums[i - k] + nums[i];
        maxSum = fmax(maxSum, sum);
    }
    return (double)(maxSum) / k;
}
