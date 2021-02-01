#include <stdio.h>
#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

//1.0
int* fairCandySwap1(int* A, int ASize, int* B, int BSize, int* returnSize)
{
    int sum1 = 0, sum2 = 0;
    for(int i = 0; i <ASize; i++)
    {
        sum1 = sum1 + A[i];
    }
    for(int j = 0; j < BSize; j++)
    {
        sum2 = sum2 + B[j];
    }
    int *ret = (int*)malloc(sizeof(int)*2);
    for(int i = 0;i<ASize; i++)
    {
        for(int j = 0; j < BSize; j++)
        {
            if(sum1-A[i] + B[j] == sum2-B[j] + A[i])
            {
                ret[0] = A[i];
                ret[1] = B[j];
            }
        }
    }

    return ret;
}

//2.0
/* 数组求和 */
int calSum(int *arr, int n) 
{
    int i;
    int sum = 0;
    for (i = 0; i < n; i++) 
    {
        sum += arr[i];
    }
    return sum;
}

/* 糖果从小到大排序 */
int cmp(const void *a, const void *b) 
{
    return *(int*)a - *(int*)b;
}

int* fairCandySwap(int* A, int ASize, int* B, int BSize, int* returnSize)
{
    int sumA = calSum(A, ASize);
    int sumB = calSum(B, BSize);
    int diff = (sumA - sumB) / 2; /* 差值需要除以2 */
    int i = 0;
    int j = 0;
    int *res = (int*)malloc(sizeof(int) * 2);
    *returnSize = 2;

    /* 糖果排序 */
    qsort(A, ASize, sizeof(A[0]), cmp);
    qsort(B, BSize, sizeof(B[0]), cmp);

    /* 双指针, 差值大于diff, j++; 差值小于diff, i++ */
    while (i < ASize && j < BSize) 
    {
        if (A[i] - B[j] == diff) 
        {
            res[0] = A[i];
            res[1] = B[j];
            return res;
        } 
        else if (A[i] - B[j] > diff)
        {
            j++;
        } 
        else 
        {
            i++;
        }
    }
    return res;
}

int main()
{
    int A[2] = {1,1};
    int B[2] = {2,2};
    int *ret = fairCandySwap1(A,2,B,2,NULL);
    for(int i = 0; i <2; i++)
    {
        printf("%d\t",ret[i]);
    }

    return 0;
}