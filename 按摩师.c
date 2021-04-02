#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//1.0
int massage(int* nums, int numsSize)
{
    if(numsSize <= 0)
    {
        return 0;
    }
    int dp[numsSize][2];
    dp[0][0] = 0;
    dp[0][1] = nums[0];
    for(int i = 1; i < numsSize;i++)
    {
        dp[i][0] = fmax(dp[i-1][0],dp[i-1][1]);
        dp[i][1] = dp[i-1][0] + nums[i];
    }
    int Max = fmax(dp[numsSize-1][0],dp[numsSize-1][1]);
    
    return Max;
}

//2.0
int massag(int* nums, int numsSize)
{
    if(numsSize <= 0)
    {
        return 0;
    }
    if(numsSize == 1)
    {
        return nums[0];
    }
    int dp[numsSize];
    dp[0] = nums[0];
    dp[1] = fmax(nums[0],nums[1]);
    for(int i = 2; i < numsSize; i++)
    {
        dp[i] = fmax(dp[i-2] + nums[i] , dp[i-1]);
    }
    return dp[numsSize-1];
}

int main()
{
    int A[100], n;
    printf("请输入数组大小:\n");
    scanf("%d",&n);
    for(int i=0; i<n; i++)
    {
        scanf("%d",&A[i]);
    }
    int max = massage(A,n);
    printf("最长预约时间为:%d\n",max);
 
    return 0;
}