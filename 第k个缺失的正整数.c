int findKthPositive(int* arr, int arrSize, int k)
{
    int ans[2001] = {0},cnt = 0,temp;
    for(int i = 0;i < arrSize;i++)
    {
        ans[arr[i]] = 1;
    }
    for(int i = 1;i < 2001;i++)
    {
        if(ans[i] != 0)
        {
            continue;
        }
        if(ans[i] == 0)
        {
            cnt++;
        }
        if(cnt == k)
        {
            temp = i;
            break;
        }
    }
    return temp;

}