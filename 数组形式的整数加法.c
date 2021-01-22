/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
//1.0数组太大超过int  和  long int 
int* addToArrayForm(int* A, int ASize, int K, int* returnSize)
{
    unsigned long int sum = 0;
    for(int i = 0;i<ASize;i++)
    {
        sum = sum*10 + A[i];
    }
    unsigned long int x = sum + K;
    unsigned long int m = x,n = 0;
    while(m>0)
    {
        n++;
        m = m/10;
    }
    *returnSize = n;
    int *ret = (int*)malloc(sizeof(int)*(*returnSize));
    if(x == 0)
    {
        ret[0] = 0;
    }
    for(int j = n-1;j>=0;j--)
    {
        ret[j] = x%10;
        x = x/10;
    }
    return ret;
}

//2.0
int* addToArrayForm(int* A, int ASize, int K, int* returnSize)
{
    int i =  ASize - 1;

    while(K != 0 && i >= 0)
    {
        A[i] += K;
        K = A[i] / 10;
        A[i] %= 10;
        i--;
    }

    if (K != 0) // 产生了新的最高位
    {
        int size = 10;
        int *pNew = (int*)malloc(sizeof(int)*size);  // 分配一块新的内存，因为0 <= K <= 10000，所以这里分配一块很小的内存
        i = size - 1;
        while ( K != 0 )
        {
            pNew[i] = K % 10;
            K /= 10;
            i--;
        }

        // 再分配一块内存，这个内存的前面放置之前分配的内存的数据，后面接上A这个数组里面的东西
        int usedSize = size-1-i;
        int *pRet = (int*)malloc(sizeof(int)*(usedSize+ASize));
        memcpy(pRet, pNew+i+1, sizeof(int)*usedSize);
        memcpy(pRet+usedSize,A,sizeof(int)*ASize);
        *returnSize = usedSize + ASize;

        return pRet;
    }


    *returnSize = ASize;
    return A;
}