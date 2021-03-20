#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* compressString(char* S)
{
    int j = 0, k = 1;
    char ch = S[0];
    if(strlen(S) == 0)
    {
        return S;
    }
    char *B = (char*)malloc(sizeof(char)*strlen(S)*2);
    for(int i = 1; i < strlen(S);i++)
    {
        if(ch == S[i])
        {
            k++;
        }
        else
        {
            B[j] = ch;
            sprintf(&B[j+1],"%d",k);
            j = j+2;
            ch = S[i];
            k = 1;
        }
    }
    B[j] = ch;
    //sprintf(&B[j+1],"%d",1);
    if(strlen(B)-2>strlen(S))
    {
        return S;
    }
    return B;
}

int main()
{
    char A[100];
    char *B = NULL;
    fgets(A, 100,stdin);
    B = compressString(A);
    fputs(B, stdout);

    return 0;
}