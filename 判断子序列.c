#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1.0超时
int isSubsequence(char * s, char * t)
{
    int i, j;
    for(i = 0;i < strlen(s);)
    {
        for(j = 0;j < strlen(t);j++)
        {
            if(s[i] == t[j])
            {
                i++;
            }
        }
    }
    if(i < strlen(s)) 
        return 0;
    else
        return 1;
    
}

//2.0通过
int isSubsequenc(char * s, char * t)
{
    int i, j;
    for(i = 0,j = 0;i < strlen(s) && j < strlen(t);j++)
    {
        if(s[i] == t[j])
        {
            i++;
        }
    }
    if(i < strlen(s)) 
        return 0;
    else
        return 1;
}

//3.0动态规划
int isSubsequence(char* s, char* t) 
{
    int n = strlen(s), m = strlen(t);

    int f[m + 1][26];
    memset(f, 0, sizeof(f));
    for (int i = 0; i < 26; i++) 
    {
        f[m][i] = m;
    }

    for (int i = m - 1; i >= 0; i--) 
    {
        for (int j = 0; j < 26; j++) 
        {
            if (t[i] == j + 'a')
                f[i][j] = i;
            else
                f[i][j] = f[i + 1][j];
        }
    }
    int add = 0;
    for (int i = 0; i < n; i++) 
    {
        if (f[add][s[i] - 'a'] == m) 
        {
            return 0;
        }
        add = f[add][s[i] - 'a'] + 1;
    }
    return 1;
}

int main()
{
    char A[100];
    char B[100];
    fgets(A, 100,stdin);
    fgets(B, 100,stdin);
    int x = isSubsequence(A, B);
    if(x == 0) printf("false");
    if(x == 1) printf("true");

    return 0;
}