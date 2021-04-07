#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char A[100];
    char **arg = (char **)malloc(sizeof(char*)*10);
    int num = 0,i = 0,j = 0,k = 0;
    fgets(A, 100,stdin);
    while(A[i] != '\0')
    {
        if(k == 0)
        {
            arg[j] = &A[i];
            k = 1;
            j++;
            num++;
        }
        while(A[i] == ' ')
        {
            k = 0;
            A[i] = '\0';
        }
        i++;
    }
    for(i = 0; i < num; i++)
    {
        printf("%s\n", arg[i]);
    }

    return 0;
}