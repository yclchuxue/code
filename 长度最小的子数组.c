#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//暴力1,sum == target       看错题了题目为sum >= target
int minSubArrayLen0(int target, int* nums, int numsSize)
{
    if(numsSize == 0)
        return 0;
    int ans = 0,min = 0;
    for(int i = 0; i < numsSize;i++)
    {
        int sum = 0;
        for(int j = i;j<numsSize;j++)
        {
            if(sum < target)
            {
                sum = sum + nums[j];
            }
            if(sum == target)
            {
                if(ans != 0 && min > j-i+1)
                {
                    min = j-i+1;
                }
                if(ans == 0)
                    min = j-i+1;
                ans++;
            }
        }
    }

    return min;
}

//暴力2 
int minSubArrayLen1(int target, int* nums, int numsSize)
{
    if(numsSize == 0)
        return 0;
    int ans = 0,min = 0;
    for(int i = 0; i < numsSize;i++)
    {
        int sum = 0;
        for(int j = i;j<numsSize;j++)
        {
            sum = sum + nums[j];
            if(sum >= target)
            {
                //printf("%d\n",j-i+1);
                if(ans != 0 && min > j-i+1)
                {
                    min = j-i+1;
                }
                if(ans == 0)
                    min = j-i+1;
                ans++;
                break;
            }
        }
    }

    return min;
}


void Sum(int *A,int* nums, int numsSize)
{
    int i,sum;
    for (i = 0; i < numsSize;i++)
    {
        sum = 0;
        for(int j = i-1; j >= 0; j--)
        {
            sum = sum + nums[j];
        }
        A[i] = sum;
    }
    for(i = 0; i < numsSize; i++)
    {
        printf("%d\t",A[i]);
    }
}

int minSubArrayLen2(int target, int* nums, int numsSize)
{
    int *A = (int*)malloc(sizeof(int) *numsSize);
    memset(A,0,sizeof(int));
    Sum(A,nums, numsSize);
    int left = 0,right = numsSize-1;
    while(left < right)
    {

    }

    return 0;
}




int lower_bound(int *a, int l, int r, int q) {
    if (a[r] < q) return -1;
    while (l < r) {
        int mid = (l + r) >> 1;
        if (a[mid] >= q) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }
    return l;
}


//题解
int minSubArrayLen3(int s, int *nums, int numsSize) {
    if (numsSize == 0) {
        return 0;
    }
    int ans = 0;
    int *sums = (int *)malloc(sizeof(int) * (numsSize + 1));
    // 为了方便计算，令 size = n + 1
    // sums[0] = 0 意味着前 0 个元素的前缀和为 0
    // sums[1] = A[0] 前 1 个元素的前缀和为 A[0]
    // 以此类推
    for (int i = 1; i <= numsSize; i++) {
        sums[i] = sums[i - 1] + nums[i - 1];
    }
    
    for(int i = 0; i < numsSize;i++)
    {
        printf("%d\t",sums[i]);
    }
    /*
    for (int i = 1; i <= numsSize; i++) {
        int target = s + sums[i - 1];
        int bound = lower_bound(sums, 1, numsSize, target);
        if (bound != -1) {
            ans = fmin(ans, bound - (i - 1));
        }
    }
    */
}

int main()
{
    int A[10] = {1,2,3,4,5};
    int m = 11,n = 5;
    int x = minSubArrayLen2(m,A,n);
    printf("\n%d",x);

    return 0;
}