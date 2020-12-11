#include <stdio.h>
int s[100];
int main()
{
    int n, p;
    scanf("%d",&n);
    for(int i = 0;i<n;i++)
    {
        scanf("%d",&s[i]);
    }

    for(int j = p;j>0;j--)
    {
        int a,b,c;
        scanf("%d",&c);
        if(c == 1)
        {
            scanf("%d",&a);
            for(int i = a;i<=n-a;i++)
            {
                s[i] = s[i+1];
            }
        }
        if(c == 2)
        {
            scanf("%d%d",&a,&b);
            for(int i = n;i>=a;i--)
            {
                s[i+1] = s[i];
            }
            s[a] = b;
        }
        if(c == 3)
        {
            scanf("%d",&a);
            int m = 1,sum = a;
            for(int i = a+1;s[i] == s[a];i++)
            {
                m++;
                sum = sum +a;
            }
            for(int h = a+1;h<=n;h++)
            {
                s[a] = sum;
                s[h] = s[h+a-1];
            }
        }
        for(int i = 0;i<n;i++)
        {
            printf("%d\t",s[i]);
        }
    }

    return 0;
}