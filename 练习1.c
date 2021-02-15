#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int maxNumberOfBalloons(char * text)
{
    int A[26] = {0};
    int n = strlen(text)-1;
    for(int i = 0; i < n; i++)
    {
        printf("%d\n",('a'-text[i]));
    }
    qsort(A,26, sizeof(int),cmp);

    return A[19];
}

int main()
{
    char str[100];
    fgets(str,100,stdin);
    int n = maxNumberOfBalloons(str);
    printf("%d", n);

    return 0;
}




nlaebolko
