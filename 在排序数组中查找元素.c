#include <stdio.h>
#include <stdlib.h>

/*暴力通过*/
int* searchRange0(int* nums, int numsSize, int target, int* returnSize)
{
    *returnSize = 2;
    int *A = (int*)malloc(sizeof(int) *2),sum = 0;
    if(numsSize == 0)
    {
        A[0] = -1;
        A[1] = -1;
    }
    for(int i = 0; i < numsSize;i++)
    {
        if(nums[i] == target)
        {
            sum++;
            if(sum == 1)
            {
                A[0] = i;
            }
            if(i == numsSize-1 || nums[i+1] != target)
            {
                A[1] = i;
            }
        }
    }
    if(sum == 0)
    {
        A[0] = -1;
        A[1] = -1;
    }
    if(sum == 1)
    {
        A[1] = A[0];
    }

    return A;
}

/*二分法(分别查找第一个目标数和第一个大于目标数的位置)*/
int* searchRange1(int* nums, int numsSize, int target, int* returnSize)
{
    int left = 0, right = numsSize-1;
    *returnSize = 2;
    int *A = (int*)malloc(sizeof(int) *2);
    if(numsSize == 0)
    {
        A[0] = -1;
        A[1] = -1;
        return A;
    }
    while(left < right)            //查找第一个出现的target位置
    {
        int mid = (left + right) / 2;
        if(nums[mid] >= target)
        {
            right = mid;
        }
        else
        {
            left = mid + 1;
        }
    }
    if(nums[left] != target)
    {
        A[0] = -1;
        A[1] = -1;
        return A;
    }
    //printf("%d\n",left);
    A[0] = left;
    left = 0;
    right = numsSize-1;
    int mid;
    while(left < right)            //查找最后一个target的数的位置
    {
        mid = (left+ right)/2;
        if(nums[mid] > target)
        {
            right = mid;
        }
        else
        {
            left = mid+1;
        }
    }
    if(nums[left]>target)
        left--;
    if(nums[left] == target)
        A[1] = left;

    return A;
}

/*二分法题解*/
int binarySearch21(int* nums, int numsSize, int target, int lower) {
    int left = 0, right = numsSize - 1, ans = numsSize;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (nums[mid] > target || (lower && nums[mid] >= target)) {
            right = mid - 1;
            ans = mid;
        } else {
            left = mid + 1;
        }
    }
    return ans;
}

int* searchRange2(int* nums, int numsSize, int target, int* returnSize) {
    int leftIdx = binarySearch21(nums, numsSize, target, 1);
    int rightIdx = binarySearch21(nums, numsSize, target, 0) - 1;
    int* ret = malloc(sizeof(int) * 2);
    *returnSize = 2;
    if (leftIdx <= rightIdx && rightIdx < numsSize && nums[leftIdx] == target && nums[rightIdx] == target) {
        ret[0] = leftIdx, ret[1] = rightIdx;
        return ret;
    }
    ret[0] = -1, ret[1] = -1;
    return ret;
}

int main()
{
    int A[6] = {5,7,7,8,8,10};
    int target = 8,m,*B;
    B = searchRange1(A,6,target,&m);
    for(int i = 0; i < m; i++)
    {
        printf("%d\t",B[i]);
    }

    return 0;
}