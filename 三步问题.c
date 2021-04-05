#include <stdio.h>
#include <stdlib.h>

int waysToStep(int n) 
{

    if(n == 3)
    {
        return 4;
    }
    else if(n < 4)
    {
        return n;
    }
    

    int dp1 = 1;
    int dp2 = 2;
    int dp3 = 4;
    int dp4 = 0;

    for(int i = 4; i <= n; ++i) 
    {
        dp4 = ((dp1 + dp2) % 1000000007 + dp3) % 1000000007;
        dp1 = dp2;
        dp2 = dp3;
        dp3 = dp4;
    }

    return dp4;
}

int main()
{
    int N;
    printf("请输入台阶个数:\n");
    scanf("%d", &N);
    int x = waysToStep(N);
    printf("有%d种走法",x);

    return 0;
}
