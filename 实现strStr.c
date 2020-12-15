int strStr(char * haystack, char * needle)
{
    if(*needle ==0)return 0;
    if(*haystack == 0)return -1;
    int m = strlen(haystack);
    int n = strlen(needle);
    int ice = 0, j = 0;
    while(*haystack)
    {
        if(*haystack == *needle)
        {
            ice = 1;
            for(int i = 0;i<n;i++)
            {
                if(m - j < n)
                {
                    return -1;
                }
                if(*(haystack+i) != needle[i])
                {
                    ice = 0;
                    break;
                }
            }
            if(ice)return j;
        }
        haystack++;
        j++;
    }
    return -1;
}