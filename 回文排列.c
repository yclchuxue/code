#include <string.h>
#include <stdio.h>

int canPermutePalindrome(char* s)
{
    int len = strlen(s);
    int A[128] = {0}, i, j, cnt;
    for(i = 0; i < len; i++)
    {
        A[s[i]]++;
    }
    for(j = 0; j < 128; j++)
    {
        if(A[j]%2 == 1)
        {
            cnt++;
        }
        if(cnt > 2)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    char s[100];
    int p;
    fgets(s, 100, stdin);
    p = canPermutePalindrome(s);
    if(p == 1)
        printf("true\n");
    else
        printf("false\n");

    return 0;
}