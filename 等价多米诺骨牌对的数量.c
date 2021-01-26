//1.0 会超时
int numEquivDominoPairs(int** dominoes, int dominoesSize, int* dominoesColSize)
{
    int sum = 0;
    for(int i = 0;i<dominoesSize;i++)
    {
        for(int j = i+1;j<dominoesSize;j++)
        {
            int x = 0;
            if(dominoes[i][0] == dominoes[j][0])
            {
                if(dominoes[i][1] == dominoes[j][1])
                {
                    x = 1;
                    sum++;
                }
            }
            if(dominoes[i][0] == dominoes[j][1])
            {
                if(dominoes[i][1] == dominoes[j][0])
                {
                    if(x != 1)
                    {
                        sum++;
                    }
                }
            }
        }
    }
    return sum;
}

//2.0
int numEquivDominoPairs(int** dominoes, int dominoesSize, int* dominoesColSize) 
{
    int num[100];
    memset(num, 0, sizeof(num));
    int ret = 0;
    for (int i = 0; i < dominoesSize; i++) 
    {
        int val = 0;
        if(dominoes[i][0] < dominoes[i][1])
        {
            val = dominoes[i][0] * 10 + dominoes[i][1];
        }
        else
        {
            val = dominoes[i][1] * 10 + dominoes[i][0];
        }
        //int val = dominoes[i][0] < dominoes[i][1] ? dominoes[i][0] * 10 + dominoes[i][1] : dominoes[i][1] * 10 + dominoes[i][0];
        ret += num[val];
        num[val]++;
    }
    return ret;
}