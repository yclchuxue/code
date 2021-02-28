#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pand(char *str,int len)
{
    for(int i = 0,j = len-1;i < j;i++,j--)
    {
        if(str[i] != str[j])
        {
            return 0;
        }
    }
    return 1;
}

char * longestPalindrome(char * s)
{
    int max = 0,x = 0,y = 0;
    int n = strlen(s) - 1;
    if(n == 1)
    {
        return s;
    }
    for(int i = 0; i < n-1 ; i++)
    {
        for(int j = i + 1;j < n; j++)
        {
            if(s[i] == s[j])
            {
                int h = pand(&s[i],j-i+1);
                if(h == 1 && (j-i+1)>max)
                {
                    max = j-i+1;
                    x = i;
                    y = j;
                }
            }
        }
    }
    if(max != 0)
    {
        char *B = (char*)malloc(sizeof(char)*max);
        for(int i = x,z = 0;i<=y;i++,z++)
        {
            B[z] = s[i];
        }
        return B;
    }
    else
    {
        char *p = (char*)malloc(sizeof(char));
        p[0] = s[0];
        return p;
        //return &s[n-1];
    }
}

int main()
{
    char A[100];
    printf("请输入字符串：");
    fgets(A, 100,stdin);
    char *p = longestPalindrome(A);
    fputs(p,stdout);

    return 0;
}

char * longestPalindrome1(char * s)
{
    char* s1,* s2,* tar;
    int i,j,length,slen;
    tar=s;
    length=1;
    slen=strlen(s);
    for(i=0;i+length<slen;i++)
    {
        for(j=i+length;j<slen;j++)
        {
            s1=s+i;
            s2=s+j;
            while(s1<s2)
            {
                if((*s1)!=(*s2))
                    break;
                s1++;
                s2--;
            }
            if(s1>=s2)
            {
                tar=s+i;
                length=j-i+1;
            }
        }
    }
    tar[length]=0;
    return tar;
}



char * longestPalindrome2(char * s)
{
    char* left,* right,* now,* tar;
    int dist,length;
    now=tar=s;
    length=1;
    while(*now)
    {
        right=now;
        left=now-1;
        do
        {
            right++;
        }while((*right)==(*now));
        now=right;
        while(left>=s&&(*right)&&((*left)==(*right)))
        {
            left--;
            right++;
        }
        dist=right-left-1;
        if(dist>length)
        {
            length=dist;
            tar=left+1;
        }
    }
    tar[length]=0;
    return tar;
}
