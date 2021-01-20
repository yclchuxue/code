//1.0负数不行
int maximumProduct(int* nums, int numsSize)
{
    for(int i = 0;i<numsSize-1;i++)
    {
        for(int j = 0;j<numsSize-i-1;j++)
        {
            if(nums[j]<nums[j+1])
            {
                int t = nums[j];
                nums[j] = nums[j+1];
                nums[j+1] = t;
            }
        }
    }

    return nums[0] * nums[1] * nums[2];
}

//2.0
int maximumProduct(int* nums, int numsSize) {
    // 最小的和第二小的
    int min1 = INT_MAX, min2 = INT_MAX;
    // 最大的、第二大的和第三大的
    int max1 = INT_MIN, max2 = INT_MIN, max3 = INT_MIN;

    for (int i = 0; i < numsSize; i++) {
        int x = nums[i];
        if (x < min1) {
            min2 = min1;
            min1 = x;
        } else if (x < min2) {
            min2 = x;
        }

        if (x > max1) {
            max3 = max2;
            max2 = max1;
            max1 = x;
        } else if (x > max2) {
            max3 = max2;
            max2 = x;
        } else if (x > max3) {
            max3 = x;
        }
    }

    return fmax(min1 * min2 * max1, max1 * max2 * max3);
}

//3.0
int cmp(int* a, int* b) {
    return *a - *b;
}

int maximumProduct(int* nums, int numsSize) {
    qsort(nums, numsSize, sizeof(int), cmp);
    return fmax(nums[0] * nums[1] * nums[numsSize - 1], nums[numsSize - 3] * nums[numsSize - 2] * nums[numsSize - 1]);
}