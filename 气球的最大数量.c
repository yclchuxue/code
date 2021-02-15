#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//1.0
int cmp (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int maxNumberOfBalloons1(char * text)
{
    float A[26] = {0};
    int n = strlen(text)-1;
    for(int i = 0; i < n; i++)
    {
        if(text[i] == 'b' || text[i] == 'a' || text[i] == 'n')
        {
            A[text[i]-'a']++;
        }
        else if(text[i] == 'l'|| text[i] == 'o')
        {
            A[text[i]-'a'] = A[text[i]-'a'] + 0.5;
        }
    }
    qsort(A,26, sizeof(int),cmp);

    return A[21];
}


//2.0
int maxNumberOfBalloons2(char * text)
{
    unsigned short index=0,b=0,a=0,l=0,o=0,n=0;
    while(text[index]!=0)
    {
        switch (text[index])
        {
            case 'b':b++;break;
            case 'a':a++;break;
            case 'l':l++;break;
            case 'o':o++;break;
            case 'n':n++;break;
        }
        index++;
    }
    index=b>a?a:b;
    index=index>(l/2)?(l/2):index;
    index=index>(o/2)?(o/2):index;
    return index>n?n:index;
}

int main()
{
    char str[100];
    fgets(str,100,stdin);
    int n = maxNumberOfBalloons1(str);
    printf("%d", n);

    return 0;
}

