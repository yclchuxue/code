#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int N,x,i = 0,j = -1,y = 1,m,n,p,q;
    scanf("%d",&N);
    int A[100][100];
    m = -1;
    n = 0;
    p = -1;
    q = 0;
    while(y <= N*N)
    {
        //printf("*%d*/%d/\n",i,j);
        for(j = j + 1;j<N-n;j++)
        {
            A[i][j] = y;
            y++;
        }
        p++;
        j--;
        for(i = i + 1;i<N-q;i++)
        {
            A[i][j] = y;
            y++;
        }
        n++;
        i--;
        for(j = j - 1;j>m;j--)
        {
            A[i][j] = y;
            y++;
        }
        q++;
        j++;
        //printf("%d\n%d\n%d\n",j,i,p);
        for(i = i - 1;i>p;i--)
        {
            A[i][j] = y;
            //printf("%3d",y);
            y++;
        }
        m++;
        i++;
    }

    for(i = 0;i<N; i++)
    {
        for(j = 0;j<N;j++)
        {
            printf("%3d",A[i][j]);
        }
        printf("\n");
    }

    return 0;
}