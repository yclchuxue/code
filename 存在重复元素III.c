#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int containsNearbyAlmostDuplicate(int* nums, int numsSize, int k, int t)
{
    int i,j;
    for(i=0;i<numsSize;i++)
    {
        for(j=1+i;j<numsSize;j++)
        {
            if(abs(nums[i]-nums[j]) <= t && abs(i-j) <= k)
            {
                printf("%d %d",i,j);
                return 1;
            }
        }
    }
    return 0;
}

int main()
{
    int A[10] = {1,2,3,1};
    int numsSize = 4;
    int k = 3,t = 0;
    int m = containsNearbyAlmostDuplicate(A, numsSize, k, t);
    printf("%d",m);

    return 0;
}