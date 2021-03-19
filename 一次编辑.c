#include <stdio.h>
#include <string.h>

int oneEditAway(char* first, char* second)
{
    int i, j, k = 0;
    int len1 = strlen(first);
    int len2 = strlen(second);
    if(len1-len2>1 || len2-len1>1)
    {
        return 0;
    }
    if(len1 > len2)
    {
        i = 0;
        j = 0;
        while(i < len1 && j < len2)
        {
            if(first[i] != second[j])
            {
                i++;
                k++;
            }
            else
            {
                i++;
                j++;
            }
        }
        if(k >= 2)
        {
            return 0;
        }
    }
    else if(len1 < len2)
    {
        i = 0;
        j = 0;
        while(i < len1 && j < len2)
        {
            if(first[i] != second[j])
            {
                j++;
                k++;
            }
            else
            {
                j++;
                i++;
            }
        }
        if(k >= 2)
        {
            return 0;
        }
    }
    else
    {
        if(len1 == 0)
        {
            return 1;
        }
        i = 0;
        j = 0;
        while(i < len1 && j < len2)
        {
            if(first[i] != second[j])
            {
                k++;
            }
            i++;
            j++;
        }
        if(k >= 2)
        {
            return 0;
        }
    }
    
    return 1;
}

int main()
{
    char A[100];
    char B[100];
    int p;
    fgets(A, 100,stdin);
    fgets(B, 100,stdin);
    p = oneEditAway(A, B);

    if(p == 1)
        printf("true\n");
    else
        printf("false\n");

    return 0;
}