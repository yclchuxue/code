#include <stdio.h>
int main()
{
    int n, k;
    int sum1 = 0, sum2 = 0;
    int m1 = 0, m2 = 0;
    scanf("%d%d", &n, &k);
    for(int i = 1;i<=n;i++)
    {
        if(i%k == 0)
        {
            sum1 = sum1 + i;
            m1++;
        }
        else
        {
            sum2 = sum2 + i;
            m2++;
        }
    }
    float s1 = (float)(sum1/m1);
    float s2 = (float)(sum2/m2);
    printf("%.1f %.1f", s1, s2);

    return 0;
}