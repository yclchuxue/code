#include <stdio.h>
#include <stdlib.h>

int maxSubArray(int* nums, int numsSize)
{
    int pre = 0, Max = nums[0];
    for (int i = 0; i < numsSize;i++)
    {
        pre = fmax(pre + nums[i],nums[i]);
        Max = fmax(pre, Max);
    }
    
    return Max;
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
    printf("最大连续数组和为:%d\n",max);
}