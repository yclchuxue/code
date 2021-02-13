int longestPalindrome(char * s)
{
    int c[128]={0},ret=0;
    
    for(int i=0;i<strlen(s);i++)
    {
        c[s[i]]++;
    }

    for(int i=0;i<128;i++)
    {
        ret+=c[i]-c[i]%2;
    }


    return ret+(ret!=strlen(s));
}



int longestPalindrome(char * s)
{
    int result=0;
    int count[52] = {0,};
    for(int i=0;s[i]!='\0';i++)
    {
        if(s[i] <= 'Z')
        {
            count[s[i]-'A'+26]++;
        }
        else
        {
            count[s[i]-'a']++;
        }
    }
    for(int i=0;i<52;i++)
    {
        result += count[i]/2*2;
    }
    for(int i=0;i<52;i++)
    {
        if(count[i]&1)
        {
            result++;
            break;
        }
    }
    return result;
 
}
