#include <stdio.h>
int s[1000];
int main()
{
    int n;
    scanf("%d",&n);
    for(int i = 0;i<n;i++)
    {
        scanf("%d",&s[i]);
    }
    for(int i = 1;i + 1<n;i++)
    {
        if(s[i-1] != s[i])
        {
            if(s[i-1] != s[i+1])
            {
                
            }
        }
    }
}