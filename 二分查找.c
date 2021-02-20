#include <stdio.h>
#include <stdlib.h>

int search(int* nums, int numsSize, int target)
{
    /* 定义左右边界 */
    int left = 0, right = numsSize - 1;
    /* 定义搜索区间为闭区间[left,right] */
    while (left <= right) 
    {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) 
        {
            left = mid + 1;
        } 
        else if (nums[mid] > target) 
        {
            right = mid - 1;
        } 
        else if (nums[mid] == target) 
        {
            return mid;
        }
    }

    return -1;
}

int main()
{
    int A[100] = {0},m,n;
    printf("请输入数组大小和数组以及要查找的数：\n");
    scanf("%d %d",&m,&n);
    for(int i = 0; i < m;i++)
    {
        scanf("%d",&A[i]);
    }
    int x = search(A,m,n);

    printf("%d",x);

    return 0;
}