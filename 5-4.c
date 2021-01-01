#include <stdio.h>
int a[10],book[10],n;

void dfs(int step,int start)
{
    if(step == n)
    {
        for(int i = 1;i<n;i++)
        {
            printf("%d",a[i]);
        }
        printf("\n");
        return ;
    }
    else
    {
        for(int i = start;i<n;i++)
        {
            if(book[i] == 0)
            {
                a[i] = i;
                book[i] = 1;
                dfs(step + 1,start+1);
                book[i] = 0;
            }
        }
    }
}


int main()
{
    scanf("%d",&n);
    dfs(1,1);

    return 0;
}