#include <stdio.h>
#include <string.h>

char* reverseWords(char* s)
{
    while(*s == ' ')
    {
        *s++;
    }
    char *q = s;
    while(*s != ' ')
    {
        *s++;
    }
    char *p = s;
    if(*s != '\0')
    {
        char *k = reverseWords(s);
    }
    *p = '\0';
    return q;
}

int main()
{
    char str[100];
    fgets(str,100,stdin);
    char *k = reverseWords(str);
    fputs(k,stdout);;

    return 0;
}