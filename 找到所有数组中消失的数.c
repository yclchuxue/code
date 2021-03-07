#include <stdio.h>
#include <stdlib.h>

int cmp (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int* findDisappearedNumbers(int* nums, int numsSize, int* returnSize)
{
    qsort(nums, numsSize, sizeof(int),cmp);
    int *ret;
    for(int i = nums[0];i<=nums[numsSize-1];i++)
    {

    }

    
}

int main()
{
    int A[100] = {0},n;
    printf("请输入数组大小：\n");
    scanf("%d",&n);
    for(int i = 0;i<n;i++)
    {
        scanf("%d",&A[i]);
    }
    int m = -1;
    int *B = findDisappearedNumbers(A,n,m);
    printf("缺失的数为：\n");
    for(int j = 0;j<m;j++)
    {
        printf("%d\t",B[j]);
    }

    return 0;
}


//2.0
int* findDisappearedNumbers(int* nums, int numsSize, int* returnSize) 
{
    for (int i = 0; i < numsSize; i++) 
    {
        int x = (nums[i] - 1) % numsSize;
        nums[x] += numsSize;
    }
    int* ret = malloc(sizeof(int) * numsSize);
    *returnSize = 0;
    for (int i = 0; i < numsSize; i++) 
    {
        if (nums[i] <= numsSize) 
        {
            ret[(*returnSize)++] = i + 1;
        }
    }
    return ret;
}