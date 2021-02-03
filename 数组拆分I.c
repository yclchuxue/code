//1.0 超时
int arrayPairSum(int* nums, int numsSize)
{
    for(int i = 0; i < numsSize-1;i++)
    {
        for(int j = 0; j < numsSize-i-1;j++)
        {
            if(nums[j]>nums[j+1])
            {
                int x = nums[j];
                nums[j] = nums[j+1];
                nums[j+1] = x;
            }
        }
    }
    int y  = 0;
    for(int i = 0; i < numsSize-1;i = i + 2)
    {
        y = y + nums[i];
    }

    return y;
}


//2.0
int arrayPairSum(int* nums, int numsSize)
{
    int sum = 0,min = nums[0];
    int *A = (int*)malloc(sizeof(int) *(100));
    memset(A, 0, sizeof(int));
    for(int i = 0; i < numsSize; i++)
    {
        A[nums[i]]++;
        if(min>nums[i])
        {
            min = nums[i];
        }
    }
    int x = 1;
    for(int i = min; i < 100; i++)
    {
        for(;A[i] != 0;A[i]--)
        {
            if(x&1 == 1)
            {
                sum = sum + i;
            }
        }
    }

    return sum;
}

//3.0
int mymin(int a,int b)
{
    return (a>b?b:a);
}
int comp(const void*a,const void*b)
{
    return *(int*)a-*(int*)b;
}
int arrayPairSum(int* nums, int numsSize)
{
    int i=0,sum=0;
    qsort(nums,numsSize,sizeof(int),comp);
    for(i=0;i<numsSize;i=i+2)
    {
        sum=sum+mymin(nums[i],nums[i+1]);
    }
    return sum;
}