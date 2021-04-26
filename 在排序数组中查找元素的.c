/*暴力*/
int* searchRange(int* nums, int numsSize, int target, int* returnSize)
{
    *returnSize = 2;
    int *A = (int*)malloc(sizeof(int) *2);
    if(numsSize == 0)
    {
        A[0] = -1;
        A[1] = -1;
    }
    for(int i = 0; i < numsSize;i++)
    {
        
    }
}

/*二分法*/
int* searchRange(int* nums, int numsSize, int target, int* returnSize)
{
    int left = 0, right = numsSize-1,*returnSize = 2;
    int *A = (int*)malloc(sizeof(int) *2);
    if(numsSize == 0)
    {
        A[0] = -1;
        A[1] = -1;
    }
    while(left < right)
    {
        int mid = left + (left+ right)/2;
        if(nums[mid] > target)
        {
            right = mid - 1;
        }
        else if(nums[mid] < target)
        {
            left = mid + 1;
        }
        else if(nums[mid] == target)
        {
            A[0] = mid;
        }
    }

}