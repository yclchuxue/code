#include <stdio.h>
int main()
{
    int m, n = 1;
    scanf("%d", &m);
    while(m != 1)
    {
        m = m/2;
        n++;
    }
    printf("%d",n);

    return 0;
}