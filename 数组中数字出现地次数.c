//错误
int* singleNumbers(int* nums, int numsSize, int* returnSize)
{
    int i,*S = (int *)malloc(sizeof(int)*2),m = 0;
    *returnSize = 2;
    for(i = 0;i < numsSize;)
    {
        if(nums[i] != nums[i+1])
        {
            S[0] = nums[i];
            S[1] = nums[i+1];
            break;
        }
        i = i + 2;
    }
    i = i +2;
    for(; i < numsSize;i++)
    {
        if(nums[i] == S[0])
        {
            i++;
            if(i >= numsSize)
            {
                S[0] = m;
                break;
            }
            S[0] = nums[i];
        }
        else if(nums[i] == S[1])
        {
            i++;
            if(i>=numsSize)
            {
                S[1] = m;
                break;
            }
            S[1] = nums[i];
        }
        else
        {
            m = nums[i];
        }
        
    }

    return S;
}

//题解
int cmp(const void *x, const void *y) {
    return *(int *)x - *(int *)y;
}
int* singleNumbers(int* nums, int numsSize, int* returnSize){
    *returnSize = 0;
    if(nums == NULL)
        return NULL;
    if(numsSize == 1) {
        (*returnSize)++;
        return nums;
    }
    qsort(nums, numsSize, sizeof(int), cmp);
    int num = 0;
    for(int i = 0; i < numsSize; i++) {
        if(i == numsSize - 1) {
            if(nums[i] != nums[i - 1])
                nums[num++] = nums[i];
                break;
        }
        if(i == 0) {
            if(nums[i] != nums[i + 1])
                nums[num++] = nums[i];
        }
        else {
            if(nums[i] != nums[i - 1] && nums[i] != nums[i + 1])
                nums[num++] = nums[i];
        }
    }
    *returnSize = num;
    return nums;
}
