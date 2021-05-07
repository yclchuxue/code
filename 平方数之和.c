#include <stdio.h>
#include <math.h>

int judgeSquareSum(int c) 
{
    for (long a = 0; a * a <= c; a++) 
    {
        double b = sqrt(c - a * a);
        if (b == (int)b) 
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    int m;
    scanf("%d",&m);
    int n = judgeSquareSum(m);
    if(n)
    {
        printf("true\n");
    }
    else printf("false\n");

    return 0;
}
