#include <stdio.h>


char * longestCommonPrefix(char ** strs, int strsSize)
{
    int i, j;
    if(strsSize == 0)
    {
        return "";
    }

    char *ch = (char *)calloc(128,sizeof(char));

    for(i = 0;i<strlen(*strs);i++)
    {
        for(j = 0;j<strsSize - 1;j++)
        {
            if(strs[j][i]!=strs[j+1][i])
            {
                return ch;
            }
        }
        ch[i] = strs[j][i];
    }
    return ch;
}

int main()
{
    char a[100];
    char b[100];
    
}