#include <stdio.h>
#include <stdlib.h>

int divisorGame(int N)
{
    int f[N + 5];

}

int main()
{
    printf("请输入N\n");
    int N;
    scanf("%d", &N);
    int X = divisorGame(N);
    if(X == 0) printf("false\n");
    if(X == 1) printf("true\n");
}