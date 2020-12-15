#include <stdio.h>
unsigned long long s[100][2];
int a[100];
int main()
{
    int n;
    scanf("%d",&n);
    for(int i = 0;i<n;i++)for(int j = 0;j<n;j++)
    {
        int m = s[j][0];
        while(m != 1)
        {
            m = m-1;
            s[j][0] = s[j][0]*m;
        }
        int t = s[j][0];
        while(t)
        {
            if(t%s[j][1] == 0)
    {
        scanf("%llu%llu", &s[i][0], &s[i][1]);
    }
    for(int j = 0;j<n;j++)
    {
        int m = s[j][0];
        while(m != 1)
        {
            m = m-1;
            s[j][0] = s[j][0]*m;
        }
        int t = s[j][0];
        while(t)
        {
            if(t%s[j][1] == 0)
            {
                a[j]++;
            }
            else
            {
                break;
            }
            t = t/s[j][1];
        }
        printf("%d\n",a[j]);
    }

    return 0;
}