//大臣的路费
#include <stdio.h>

int a[1000][1000];
int flag[1000];
int Max=0;
int sum=0;
void dfs(int s, int n)
{
	int i;
	for(i=1;i<=n;i++)
	{
		if(flag[i]==0&&a[s][i]>0)
		{
			flag[i]=1;
			sum=sum+a[s][i];
			dfs(i,n);
			sum=sum-a[s][i];
			flag[i]=0;
		}
	}
	if(i==n+1)
	{
		if(sum>Max)
			Max=sum;
	}
}

int main()
{	
	int n,b,c,i;
	scanf("%d",&n);
	int money;
	for(i=0;i<n-1;i++)
	{
		scanf("%d%d",&b,&c);
		scanf("%d",&a[b][c]);
		a[c][b]=a[b][c];
	}
	for(i=1;i<=n;i++)
	{
		flag[i]=1;
		dfs(i,n);
		flag[i]=0;
	}
	money=Max*10+(1+Max)*Max/2;
	printf("%d",money);
	return 0;
} 
