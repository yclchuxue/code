#include <stdio.h>
int A[1000];


int minCostClimbingStairs(int* cost, int costSize) 
{
    int dp[costSize + 1];
    dp[0] = dp[1] = 0;
    for (int i = 2; i <= costSize; i++) 
    {
        dp[i] = fmin(dp[i - 1] + cost[i - 1], dp[i - 2] + cost[i - 2]);
    }
    return dp[costSize];
}

int main()
{
    int n;
    scanf("%d", &n);
    for(int i = 0;i<n;i++)
    {
        scanf("%d",&A[i]);
    }
    int x = minCostClimbingStairs(A,n);
    printf("%d",x);

    return 0;
}