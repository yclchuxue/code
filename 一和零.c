int findMaxForm(char ** strs, int strsSize, int m, int n) 
{
    int dp[m + 1][n + 1];
    memset(dp, 0, sizeof(int) * (m + 1) * (n + 1));
    for (int i = 0; i < strsSize; ++i) 
    {
        int count0 = 0;
        int count1 = 0;
        char *str = strs[i];
        while (*str != '\0') 
        {
            if (*str == '0') 
            {
                ++count0;
            } 
            else 
            {
                ++count1;
            }
            ++str;
        }
        
        for (int j = m; j >= count0; --j) 
        {
            for (int k = n; k >= count1; --k) 
            {
                int tmp = dp[j - count0][k - count1] + 1;
                if (tmp > dp[j][k]) 
                {
                    dp[j][k] = tmp;
                }
            }
        }
    }
    
    return dp[m][n];
}