int findLengthOfLCIS(int* nums, int numsSize) {
    int ans = 0;
    int start = 0;
    for (int i = 0; i < numsSize; i++) {
        if (i > 0 && nums[i] <= nums[i - 1]) {
            start = i;
        }
        ans = fmax(ans, i - start + 1);
    }
    return ans;
}