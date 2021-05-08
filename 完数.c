#include <stdio.h>

int factorsum( int number );
void PrintPN( int m, int n );

int main()
{
    int m, n;

    scanf("%d %d", &m, &n);
    if ( factorsum(m) == m ) printf("%d is a perfect number\n", m);
    if ( factorsum(n) == n ) printf("%d is a perfect number\n", n);
    PrintPN(m, n);

    return 0;
}

int factorsum( int number )
{
    int sum = 0,i;
    for(i = 1;i<number;i++)
    {
        if((number/i)*i == number)
        {
            sum = sum + i;
        }
    }

    return sum;
}

void PrintPN( int m, int n )
{
    int i, j, sum = 0;
    for(i = m;i<=n;i++)
    {
        if(factorsum(i) == i)
        {
            sum++;
            printf("%d = 1", i);
            for(j = 2;j<i;j++)
            {
                if((i/j)*j == i)
                {
                    printf(" + %d",j);
                }
            }
            printf("\n");
        }
    }
    if(sum == 0)
    {
        printf("No perfect number");
    }
}