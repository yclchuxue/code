#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* build(char* str) 
{
    int n = strlen(str), len = 0;
    char* ret = malloc(sizeof(char) * (n + 1));
    for (int i = 0; i < n; i++) 
    {
        if (str[i] != '#') 
        {
            ret[len++] = str[i];
        } 
        else if (len > 0) 
        {
            len--;
        }
    }
    ret[len] = '\0';
    return ret;
}

char *backspaceCompare(char* S, char* T) 
{
    char *p = "true";
    char *q = "false";
    if(strcmp(build(S), build(T)) != 0)
    {
        return q;
    }
    return p;
}


int main()
{
    char S[200], T[200];
    fgets(S, 200,stdin);
    fgets(T, 200,stdin);
    char *p = backspaceCompare(S, T);
    printf("%s",p);

    return 0;
}