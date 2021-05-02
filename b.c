#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int m,i = 0,ans = 0;
    char str[20][10];
    char sh[10];
    int size[20];
    while(strcmp(sh,"#") != 0)
    {
        scanf("%s",str[i]);
        strcpy(sh,str[i]);
        size[i] = strlen(str[i]);
        i++;
    }
    i = i-1;
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 1)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 2)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 3)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 4)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 5)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 6)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 7)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 8)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 9)
            printf("%s\n",str[j]);
    }
    for(int j = 0;j<i;j++)
    {
        if(size[j] == 10)
            printf("%s\n",str[j]);
    }
    return 0;
}